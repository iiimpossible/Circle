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
	
	// �����Ӧ����Ⱦ״̬
	CircleEngine::UseProgram(mShaderHandle);
	CircleEngine::SetVec3(mShaderHandle, "textColor", aParam.Color);	 
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVAO);

	// �����ı������е��ַ�
	std::string::const_iterator c;
	float tX = aParam.Position.x;//��Ϊ��һ����ĸ��ԭ��
	for (c = aParam.TextLine.begin(); c != aParam.TextLine.end(); c++)
	{
		 
		Character ch = mCharacters[*c];//���ַ�������ȡ�ַ����������в������Σ�

		float xpos = tX + ch.Bearing.x * aParam.Scale;//����x������,bearing�Ǵӻ�׼�ߵ�������/������ƫ��ֵ
		float ypos = aParam.Position.y - (ch.Size.y - ch.Bearing.y) * aParam.Scale;//����y������

		float w = ch.Size.x * aParam.Scale;//�����ֿ�
		float h = ch.Size.y * aParam.Scale;//�����ָ�
		// ��ÿ���ַ�����VBO
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },//��������+��������
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// ���ı����ϻ�����������
		glBindTexture(GL_TEXTURE_2D, ch.TextureHandle);
		// ����VBO�ڴ������
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// �����ı���
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64���أ�advance��ԭ�����һ������ԭ��ľ���		
		tX += (ch.Advance >> 6) * aParam.Scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
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

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������
	for (unsigned char c = 0; c < 128; c++)
	{
		// �����ַ������� 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			DebugLog("ERROR::FREETYTPE: Failed to load Glyph");

			continue;
		}
		// ��������
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
			face->glyph->bitmap.buffer//λͼ����
		);
		// ��������ѡ��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//int a = face->glyph->advance.x;
		// �����ַ���֮��ʹ��
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
 
