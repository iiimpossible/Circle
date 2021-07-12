#include <GL/glew.h>
#include "TextRender.h"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <freetype/fterrors.h>
#include "../../CCCommon/OpenGL/CCGLDraw.h"
using CircleEngine::CreateVBO;
using CircleEngine::CreateVAO;
using CircleEngine::CreateVBOEmpty;
using CircleEngine::ShaderCompile;
using CircleEngine::ReWriteVBO;
using CircleEngine::ViewportHeight;
using CircleEngine::ViewportWidth;

vector<TextLineParam> TextRender::mTextLine;
map<char, Character> TextRender::mCharacters;
unsigned TextRender::mShaderHandle;
unsigned TextRender::mVAO;
unsigned TextRender::mVBO;


const char* Text_vs = ROW(
#version 330 core\n
layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}
);

const char* Text_fs = ROW(
#version 330 core\n
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	color = vec4(textColor, 1.0) * sampled;
}
);

float TextQuadVerCoord[12]
{
	1.0f,1.0f,
	-1.0f,-1.0f,
	1.0f,-1.0f,

	1.0f,1.0f,
	-1.0f,1.0f,
	-1.0f,-1.0f
};

float TextQuadTexCoord[12]
{
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,0.0f,

	1.0f,1.0f,
	0.0f,1.0f,
	0.0f,0.0f
};


TextRender::TextRender()
{
	this->mVAO = CreateVAO();
	this->mVBO = CreateVBOEmpty(mVAO, 6 * 4, 4, 4);
	 
	this->mShaderHandle = ShaderCompile(Text_vs, Text_fs,nullptr);

	CircleEngine::Mat4 projection = glm::ortho(0.0f, ViewportWidth, 0.0f, ViewportHeight);
	CircleEngine::UseProgram(mShaderHandle);
	CircleEngine::SetMat4(mShaderHandle,"projection", projection);
	 

	InitFreeType();
}

TextRender::~TextRender()
{
}

void TextRender::Update()
{
	for (auto& val : mTextLine)
	{
		DrawTextLine(val);
	}
	mTextLine.clear();
}

 

void TextRender::RenderTextLine(const string & TextLine, Vec2 Position, float aScale, Vec3 aColor)
{	
	mTextLine.push_back(TextLineParam{TextLine, Position, aScale, aColor});
}

void TextRender::DrawTextLine(const TextLineParam & aParam)
{
	
	// 激活对应的渲染状态
	CircleEngine::UseProgram(mShaderHandle);
	CircleEngine::SetVec3(mShaderHandle, "textColor", aParam.Color);	 
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVAO);

	// 遍历文本中所有的字符
	std::string::const_iterator c;
	float tX = aParam.Position.x;//作为下一个字母的原点
	for (c = aParam.TextLine.begin(); c != aParam.TextLine.end(); c++)
	{
		 
		Character ch = mCharacters[*c];//从字符串中提取字符，在容器中查找字形，

		float xpos = tX + ch.Bearing.x * aParam.Scale;//计算x轴坐标,bearing是从基准线到字形左部/顶部的偏移值
		float ypos = aParam.Position.y - (ch.Size.y - ch.Bearing.y) * aParam.Scale;//计算y轴坐标

		float w = ch.Size.x * aParam.Scale;//计算字宽
		float h = ch.Size.y * aParam.Scale;//计算字高
		// 对每个字符更新VBO
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },//顶点坐标+纹理坐标
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// 在四边形上绘制字形纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureHandle);
		// 更新VBO内存的内容
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 绘制四边形
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 更新位置到下一个字形的原点，注意单位是1/64像素，advance是原点距下一个字形原点的距离		
		tX += (ch.Advance >> 6) * aParam.Scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRender::InitFreeType()
{
	FT_Library ft;
	FT_Face face;
	//error = FT_Init_FreeType(&ft);
	if (FT_Init_FreeType(&ft))
		DebugLog("ERROR::FREETYPE: Could not init FreeType Library");

	
	if (FT_New_Face(ft, "Asset/Font/arial.ttf", 0, &face))
	{
		DebugLog("ERROR::FREETYPE: Failed to load font");
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
	for (unsigned char c = 0; c < 128; c++)
	{
		// 加载字符的字形 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			DebugLog("ERROR::FREETYTPE: Failed to load Glyph");

			continue;
		}
		// 生成纹理
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer//位图数据
		);
		// 设置纹理选项
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//int a = face->glyph->advance.x;
		// 储存字符供之后使用
		Character character = {
			texture,
			Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		mCharacters.insert(pair<char,Character>(c,character));
		 
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
 
