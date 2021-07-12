#define GLEW_STATIC
#include <GL/glew.h>
#include "CCGLDraw.h"
#include "../Common.h"
//#include "../../CCEngine/Manager/EventManager.h"
//================================================================

const char* CircleEngine::line_vretext_shader = ROW(

#version 330 core \n
layout(location = 0) in vec3 position;
uniform mat4 projection;
void main()
{
	gl_Position = projection * vec4(position.x, position.y, 0.0f, 1.0f);
}

);

const char* CircleEngine::line_fragment_shader = ROW(

#version 330 core \n
out vec4 Fragcolor;

uniform vec3 color;
void main()
{
	Fragcolor = vec4(color.x, color.y, color.z, 1.0f);
}

); 

//================================================================ 

const char* CircleEngine::rectangle_vertex_shader = ROW(

#version 330 core \n
layout(location = 0) in vec3 position;
uniform mat4 projection;
void main()
{
	//gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
	gl_Position = projection * vec4(position.x, position.y, 0.0f, 1.0f);
}

);


const char* CircleEngine::rectangle_fragment_shader = ROW(

#version 330 core \n
out vec4 Fragcolor;

uniform vec3 color;
void main()
{
	Fragcolor = vec4(color.x, color.y, color.z, 1.0f);
}

); 

//================================================================


bool CircleEngine::InitGlew()
{
	glewExperimental = true;
	glewInit();
	if (!glewInit() == GLEW_OK)
	{
		//DebugLog("@GLWindow::InitWindow: ERROR: GLEW init failed!"); 
		return false;
		//return -1;
	}
	return true;
}

void CircleEngine::DrawWithIndices(unsigned aVAO, int aIndicesSize)
{
	glBindVertexArray(aVAO);
	glDrawElements(GL_TRIANGLES, aIndicesSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned CircleEngine::ShaderCompile(const char* vShader, const char* fShadher, const char* gShader)
{
	/*
1.创建顶点和片元着色器（拿ID）
2.读取源文件
3.编译源文件
4.创建shader程序，链接代码
*/

	unsigned vertex, fragment, geometry;
	unsigned programID;
	//创建着色器
	int success;
	char infoLog[512];


	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShader, nullptr);
	glCompileShader(vertex);

	//顶点着色器错误处理

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "[DebugLog] @ShaderCompile: ERROR:Vertex Shader Compiling Failed!\n" << infoLog << std::endl;
		//EventManager::OnShaderCompileError(0);
	}
	  
	//片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShadher, nullptr);
	glCompileShader(fragment);

	//片元着色器错误处理
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "[DebugLog] @ShaderCompile: ERROR:Fragment Shader Compiling Failed!\n" << infoLog << std::endl;
		//EventManager::OnShaderCompileError(0);
	} 

	//获得着色器把手
	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);

	if (gShader != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShader, nullptr);
		glCompileShader(geometry);
		glAttachShader(programID, geometry);
	}
	//链接
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "[DebugLog] @ShaderCompile: ERROR:Shader Linking Failed!\n" << infoLog << std::endl;
		//EventManager::OnShaderCompileError(0);

	} 
	 
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return programID;
}

unsigned CircleEngine::CreateVAO()
{
	unsigned VAO;
	glGenVertexArrays(1, &VAO);

	return VAO;
}
/**
 * @brief 将顶点信息写入显存
 * @param vertices 顶点数组
 * @param total_data_size_of_float 数据总共几个float(顶点数组总大小)
 * @param data_sets_menber_num 一组数据由几个成员组成
 * @param data_size_of_float 一组数据总共几个float(几维向量)
 * @param offset 数据分布间隔
 * @param layout 顶点信息层次 layout belong to [0,16]
 * @return VAO　顶点数组对象ID
 * @node 仅适用于float数组
 * TODO：将创建顶点数组对象，顶点缓冲对象分开，这样就能够方便的输入数据了，
 要对一个顶点缓冲对象输入数据再次绑定就好了，还有是否动态存储
 */
unsigned CircleEngine::CreateVBO(
	unsigned VAO,
	float* vertices,
	unsigned data_size,//数据总共几个float,分配总内存
	unsigned vector_demension,//一个位置向量（纹理坐标，法向量）的维度
	unsigned stride,//指定连续顶点属性之间的偏移量
	unsigned offset,//数据每隔几个
	unsigned layout)
{
	unsigned VBO; 
	glGenVertexArrays(1, &VBO);
	glGenBuffers(1, &VBO); 
	 
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data_size * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, vector_demension, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*) offset);
	glEnableVertexAttribArray(layout);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  

	glBindVertexArray(0);   
	return VBO;
}

/**
* @brief 分批顶点属性。
* @param[in] VAO 顶点数组对象句柄
* @param[in] vertex 顶点数据及其格式信息。按照顺序：<0>数组指针，<1>数组大小，<2>单个数据维度，<3>步幅
* @param[in] tex_coord 纹理坐标数组及其信息。
*/
 unsigned CircleEngine::CreateVBO(unsigned aVAO, tuple<float*, unsigned, unsigned, unsigned> aVertexInfo, tuple<float*, unsigned, unsigned, unsigned> aTexCoordInfo)
 {
	 unsigned VBO; 

	 float* Position = std::get<0>(aVertexInfo);
	 unsigned PositionSize = std::get<1>(aVertexInfo);
	 unsigned PositionDemension = std::get<2>(aVertexInfo);
	 unsigned PositionStride = std::get<3>(aVertexInfo);

	 float* Texture = std::get<0>(aTexCoordInfo);
	 unsigned TextureSize = std::get<1>(aTexCoordInfo);
	 unsigned TextureDemenSion = std::get<2>(aTexCoordInfo);
	 unsigned TextureStride = std::get<3>(aTexCoordInfo);

	 unsigned DataSize = PositionSize + TextureSize;

	 //创建一个顶点数组和一个空的顶点缓冲
	 glGenVertexArrays(1, &VBO);
	 glGenBuffers(1, &VBO);
	 glBindVertexArray(aVAO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, DataSize * sizeof(float), nullptr, GL_STATIC_DRAW);

	 //给顶点缓冲填入数据
	 glBufferSubData(GL_ARRAY_BUFFER, 0, PositionSize * sizeof(float), Position);
	 glBufferSubData(GL_ARRAY_BUFFER, PositionSize * sizeof(float), TextureSize * sizeof(float), Texture);

	 //告诉顶点数组顶点缓冲的格式
	 glVertexAttribPointer(0, PositionDemension, GL_FLOAT, GL_FALSE, PositionStride * sizeof(float), (void*)0);
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(1, TextureDemenSion, GL_FLOAT, GL_FALSE, TextureStride * sizeof(float), (void*)(PositionSize * sizeof(float)));//注意纹理坐标读取起点偏移
	 glEnableVertexAttribArray(1);

	 //解绑顶点数组,顶点缓冲
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);
	  
	 return VBO;
 }
 unsigned CircleEngine::CreateVBO(unsigned aVAO, tuple<float*, unsigned, unsigned, unsigned> aVertexInfo, tuple<float*, unsigned, unsigned, unsigned> aTexCoordInfo, tuple<float*, unsigned, unsigned, unsigned> aNormalInfo)
 {
	 unsigned VBO;

	 float* Position = std::get<0>(aVertexInfo);
	 unsigned PositionSize = std::get<1>(aVertexInfo);
	 unsigned PositionDemension = std::get<2>(aVertexInfo);
	 unsigned PositionStride = std::get<3>(aVertexInfo);

	 float* Texture = std::get<0>(aTexCoordInfo);
	 unsigned TextureSize = std::get<1>(aTexCoordInfo);
	 unsigned TextureDemenSion = std::get<2>(aTexCoordInfo);
	 unsigned TextureStride = std::get<3>(aTexCoordInfo);

	 float* Normal = std::get<0>(aNormalInfo);
	 unsigned NormalSize = std::get<1>(aNormalInfo);
	 unsigned NormalDemenSion = std::get<2>(aNormalInfo);
	 unsigned NormalStride = std::get<3>(aNormalInfo);

	 unsigned DataSize = PositionSize + TextureSize + NormalSize;

	 //创建一个顶点数组和一个空的顶点缓冲
	 glGenVertexArrays(1, &VBO);
	 glGenBuffers(1, &VBO);
	 glBindVertexArray(aVAO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, DataSize * sizeof(float), nullptr, GL_STATIC_DRAW);

	 //给顶点缓冲填入数据
	 glBufferSubData(GL_ARRAY_BUFFER, 0, PositionSize * sizeof(float), Position);
	 glBufferSubData(GL_ARRAY_BUFFER, PositionSize * sizeof(float), TextureSize * sizeof(float), Texture);
	 glBufferSubData(GL_ARRAY_BUFFER, (PositionSize + TextureSize) * sizeof(float), NormalSize * sizeof(float), Normal);

	 //告诉顶点数组顶点缓冲的格式
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(0, PositionDemension, GL_FLOAT, GL_FALSE, PositionStride * sizeof(float), (void*)0);
	
	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(1, NormalDemenSion, GL_FLOAT, GL_FALSE, NormalStride * sizeof(float), (void*)((PositionSize + TextureSize) * sizeof(float)));
	
	 glEnableVertexAttribArray(2);
	 glVertexAttribPointer(2, TextureDemenSion, GL_FLOAT, GL_FALSE, TextureStride * sizeof(float), (void*)(PositionSize * sizeof(float)));//注意纹理坐标读取起点偏移
	
	 //解绑顶点数组,顶点缓冲
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);

	 return VBO;
 }

 /*
 结构体的另外一个很好的用途是它的预处理指令offsetof(s, m)，它的第一个参数是一个结构体，
 第二个参数是这个结构体中变量的名字。这个宏会返回那个变量距结构体头部的字节偏移量(Byte Offset)。
 */
 //unsigned CircleEngine::CreateVBO(unsigned aVAO, const Mesh & aMeshInfo)
 //{

	// unsigned VBO;
	// glGenBuffers(1, &VBO);

	// glBindVertexArray(aVAO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData(GL_ARRAY_BUFFER, aMeshInfo.Vertices.size() * sizeof(Vertex), &aMeshInfo.Vertices[0], GL_STATIC_DRAW);//这个static需要改成可以变化的
	// 
	// // 顶点位置
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// 
	// // 顶点法线
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// // 顶点纹理坐标
	// glEnableVertexAttribArray(2);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	// glBindVertexArray(0);

	// return 0;
 //}
/**
 * @brief 创建一个空的分配了指定内存空间的VBO
 * @param total_data_size_of_float 数据总共几个float(顶点数组总大小)
 * @param data_sets_menber_num 一组数据由几个成员组成
 * @param data_size_of_float 一组数据总共几个float(几维向量)
 * @param offset 数据分布间隔
 * @param layout 顶点信息层次 layout belong to [0,16]
 * @return VBO　顶点数组对象ID
 * @note 仅适用于float数组，仅适用于动态顶点存储 
 */
unsigned CircleEngine::CreateVBOEmpty(unsigned VAO, unsigned aDataSizeOfFloat, unsigned data_sets_menber_num, unsigned data_size_of_float, unsigned offset, unsigned layout)
{
	unsigned VBO;

	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO); 

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//分配指定大小显存，但是不输入数据
	glBufferData(GL_ARRAY_BUFFER, aDataSizeOfFloat * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	//不指定顶点属性OpenGL就不知道怎么读取数据
	glVertexAttribPointer(0, data_sets_menber_num, GL_FLOAT, GL_FALSE, data_size_of_float * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VBO;
}
/**
 * @brief 将顶点索引信息写入显存
 * @param total_data_size_of_float 数据总共几个float(顶点数组总大小)
 * @param data_sets_menber_num 一组数据由几个成员组成
 * @param layout 顶点索引存储层次 layout belong to [0,16]
 * @node 仅适用于float数组
 */
unsigned CircleEngine::CreateEBO(unsigned aVAO, unsigned * aIndices, unsigned aDataSizeOfFloat, unsigned aLayout)
{
	unsigned EBO;

	glBindVertexArray(aVAO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, aDataSizeOfFloat * sizeof(float), aIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, aLayout);

	glBindVertexArray(0);


	return EBO;
}
unsigned CircleEngine::CreateEBO(unsigned aVAO, const ElemenBuffertInfo & aInfo)
{
	unsigned EBO;
	glBindVertexArray(aVAO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, aInfo.ElementSize * sizeof(float), aInfo.ElementPtr, aInfo.RenderMode);

	glBindVertexArray(0);

	return EBO;
}
/**
* @brief 重置VBO数据或者重写一部分
* @param VBO 之前创建的VBO
* @param new_vertices 新的数组
* @param start_index 重写起始点，默认为0，即全部重写
*/
void CircleEngine::ReWriteVBO(unsigned aVBO, float * aNewVertices, unsigned aDataSizeOfFloat, unsigned aStartIndex)
{
	glBindBuffer(GL_ARRAY_BUFFER, aVBO);
	glBufferSubData(GL_ARRAY_BUFFER, aStartIndex, aDataSizeOfFloat * sizeof(float), aNewVertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void CircleEngine::UseProgram(const unsigned int shaderId)
{
	glUseProgram(shaderId);
}

void CircleEngine::BindTexture(unsigned aShaderHandle, unsigned aTextureHandle, const char * aTextureName, unsigned aTextureUnit, unsigned aTextureTarget)
{
	if (aTextureUnit > 15)
	{
		//DebugLog("@CC_Common::BindTexture: Only 16 Texture Units!");
		return;
	}
	glActiveTexture(GL_TEXTURE0 + aTextureUnit);
	glBindTexture(aTextureTarget, aTextureHandle);
	glUniform1i(glGetUniformLocation(aShaderHandle, aTextureName), 0);
}

//void CircleEngine::SetBool(const unsigned int shaderId, const string &name, bool value)
//{
//	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), (int)value);
//}
//
//// ------------------------------------------------------------------------
//void CircleEngine::SetInt(const unsigned int shaderId, const string &name, int value)
//{
//	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
//}
//void CircleEngine::SetInt(const unsigned int aShaderID, const string & aName, unsigned aElmentNum, int* aValue)
//{
//	glUniform1iv(glGetUniformLocation(aShaderID, aName.c_str()), aElmentNum, aValue);
//}
//// ------------------------------------------------------------------------
//void CircleEngine::SetFloat(const unsigned int shaderId, const string &name, float value)
//{
//	glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
//}
//
//void CircleEngine::SetFloat(const unsigned int aShaderID, const string & aName, unsigned aElmentNum, float* aValue)
//{
//	glUniform1fv(glGetUniformLocation(aShaderID, aName.c_str()), aElmentNum, aValue);
//}
//
//void CircleEngine::SetVec2(const unsigned int aShaderID, const string & aName, const Vec2 & aValue)
//{
//	glUniform2fv(glGetUniformLocation(aShaderID, aName.c_str()), 1, &aValue[0]);
//}
//
//
//void CircleEngine::SetVec2(const unsigned int aShaderID, const string & aName, unsigned aElementNum, const Vec2 & aValue)
//{
//	glUniform2fv(glGetUniformLocation(aShaderID, aName.c_str()), aElementNum, &aValue[0]);
//}
//
//void CircleEngine::SetVec3(const unsigned int shaderId, const string &name, const Vec3 &value)
//{
//	glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
//}
//
//void CircleEngine::SetVec4(const unsigned int shaderId, const string &name, const Vec4 &value)
//{
//	glUniform4fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
//}
//
//void CircleEngine::SetMat2(const unsigned int shaderId, const string &name, const Mat2 &mat)
//{
//	glUniformMatrix2fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//}
//
//void CircleEngine::SetMat3(const unsigned int shaderId, const string &name, const Mat3 &mat)
//{
//	glUniformMatrix3fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//}
//
//void CircleEngine::SetMat4(const unsigned int shaderId, const string &name, const Mat4 &mat)
//{
//	glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//}



//
//Texture2DInfo CircleEngine::CreateTexture2D(unsigned char * aData, int aWidth, int aHeight, int aChannels,
//	ETextureAround aTextureAroundS,
//	ETextureAround aTextureAroundT,
//	ETextureFilter aTextureFilterMIN,
//	ETextureFilter aTextureFilterMGA)
//{
//	unsigned texutureHandle;
//	 
//		glGenTextures(1, &texutureHandle);
//
//#ifdef DEBUGLOG
//	DebugData("@CreateTexture2D: Texture ID is:", texutureHandle);
//#endif // DEBUGLOG
//
//	glBindTexture(GL_TEXTURE_2D, texutureHandle);
//
//	if (aChannels == 4)
//	{	//第二个参数是多级渐远纹理级别，0为基本级别。尺寸后边的0是历史遗留问题，都是0值
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aWidth, aHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, aData);
//	}
//	else if (aChannels == 3)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aWidth, aHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, aData);
//	}
//	else
//	{
//		
//		return {};
//	}
//
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	//配置纹理参数，2D纹理类型，s方向，t方向环绕,如果是3D纹理还有一个r,与x,y,z等价。重复环绕
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, aTextureAroundS);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, aTextureAroundT);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, aTextureFilterMIN);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, aTextureFilterMGA);
//	
//	
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	 
//	
//	return Texture2DInfo{ texutureHandle,aWidth ,aHeight ,aChannels };
//}
//
//Texture2DInfo CircleEngine::CreateTexture2D(const Texture2DParam & aParam)
//{
//	unsigned texutureHandle;
//	glGenTextures(1, &texutureHandle);
//#ifdef DEBUGLOG
//	DebugData("@CreateTexture2D: Texture ID is:", texutureHandle);
//#endif // DEBUGLOG
//
//	glBindTexture(GL_TEXTURE_2D, texutureHandle);
//	if (aParam.Channel== 4)
//	{	//第二个参数是多级渐远纹理级别，0为基本级别。尺寸后边的0是历史遗留问题，都是0值
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aParam.Width, aParam.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, aParam.TextureData);
//	}
//	else if (aParam.Channel == 3)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aParam.Width, aParam.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, aParam.TextureData);
//	}
//	else
//	{
//
//		return {};
//	}
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	//配置纹理参数，2D纹理类型，s方向，t方向环绕,如果是3D纹理还有一个r,与x,y,z等价。重复环绕
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, aParam.AroundS);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, aParam.AroundT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, aParam.FilterMIN);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, aParam.FilterMGA);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	return Texture2DInfo{ texutureHandle,aParam.Width ,aParam.Height ,aParam.Channel};
//}



//
//unsigned CircleEngine::CreateFramebuffer()
//{
//
//	unsigned frambuffer;
//	glGenFramebuffers(1, &frambuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, frambuffer);
//	return frambuffer;
//
//	
//}
//unsigned CircleEngine::CreateFramebuffer(unsigned aTextureHandle)
//{
//	unsigned frambuffer;
//	glGenFramebuffers(1, &frambuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, frambuffer);
//
//
//	return frambuffer;
//}
//unsigned CircleEngine::CreateTextureBuffer(unsigned aWidth, unsigned aHeight)
//{
//	unsigned TexColorBuffer;
//	glGenTextures(1, &TexColorBuffer);
//	glBindTexture(GL_TEXTURE_2D, TexColorBuffer);
//	//注意当前视口的宽度和高度和给这个纹理的值是空的
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aWidth, aHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);//将纹理绑定到帧缓冲
//	return TexColorBuffer;
//}
//
//unsigned CircleEngine::CreateMSColorbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum)
//{
//	return 0;
//}
//
//unsigned CircleEngine::CreateRenderBuffer(unsigned aWidth, unsigned aHeight)
//{
//	unsigned int RBO;
//
//	//创建渲染缓冲对象，GL_DEPTH24_STENCIL8是深度值和模板值的精度，4 是采样点数量
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, aWidth, aHeight);
//	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//	return RBO;
//}
//
///**
//* @brief 作为一个多重采样的渲染缓冲对象，绑定到帧缓冲GL_COLOR_ATTACHMENT0，且内存为GL_RGB格式
//*/
//unsigned CircleEngine::CreateMSRenderBuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum)
//{
//	unsigned int RBO;
//
//	//创建渲染缓冲对象，GL_DEPTH24_STENCIL8是深度值和模板值的精度
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_RGB, aWidth, aHeight);
//	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
//	return RBO;
//}
//
//
//void CircleEngine::CheckFrameBuffer()
//{
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		std::cout << "[DebugLog] @CC_Common::CreateEmptyTexture: ERROR: Framebuffer is not complete!" << std::endl;
//	}
//}
//
//void CircleEngine::UnBindFrameBuffer()
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
// 
// CircleEngine::TFrameBufferID CircleEngine::CreateTextureBuffer(TTextureParam aTextureParam)
//{
//	// 生成纹理
//	unsigned FrameBuffer;
//	FrameBuffer = CreateFramebuffer();
//	unsigned TexColorBuffer;
//	glGenTextures(1, &TexColorBuffer);
//	glBindTexture(GL_TEXTURE_2D, TexColorBuffer);
//	//注意当前视口的宽度和高度和给这个纹理的值是空的
//	glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, aTextureParam.Width, aTextureParam.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	//绑定纹理附件到帧缓冲，将它附加到GL_COLOR_ATTACHMENT0,最后的0是多级渐远纹理的级别，这里为0
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);
//	//检查帧缓冲是否是完整的，否则打印错误信息
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//删除失败帧缓冲
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout << "[DebugLog] @CC_Common::CreateTextureBuffer: ERROR: Framebuffer is not complete!" << std::endl;
//		//如果帧缓冲创建失败，返回默认的帧缓冲对象
//		return TFrameBufferID();
//	}	
//	std::cout << "[DebugLog] @CC_Common::CreateTextureBuffer: RenderBuffer Created!" << std::endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	
//	//DebugData("@CC_Common::CreateTextureBuffer: TextureBuffer ID is: ", TexColorBuffer);
//	TFrameBufferID FrameBufferID;
//	FrameBufferID.FrambufferID = FrameBuffer;
//	FrameBufferID.AttachmenID = TexColorBuffer;
//	return FrameBufferID;
//}
//
// CircleEngine::TFrameBufferID CircleEngine::CreateDepthTextureBuffer(TTextureParam aTextureParam)
// {
//	 unsigned depthBuffer;
//	 unsigned frameBuffer;
//	 frameBuffer = CreateFramebuffer();
//
//	 glGenTextures(1, &depthBuffer);
//	 glBindTexture(GL_TEXTURE_2D, depthBuffer);
//	 //注意当前视口的宽度和高度和format = depth component, type = float
//	 glTexImage2D(GL_TEXTURE_2D, 0, aTextureParam.Format, 
//		 aTextureParam.Width, aTextureParam.Height, 0, 
//		 aTextureParam.Format, aTextureParam.DataType, NULL);
//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	 glBindTexture(GL_TEXTURE_2D, 0);
//
//	 glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//	 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
//	 glDrawBuffer(GL_NONE);
//	 glReadBuffer(GL_NONE);
//	 glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	 return TFrameBufferID();
// }
//
// //创建一个带有多重采样纹理附件的多重采样帧缓冲帧缓冲， 
// CircleEngine::TFrameBufferID CircleEngine::CreateMSColorbuffer(TTextureParam aTextureParam, unsigned aSampleNum)
//{
//	unsigned FrameBuffer;	 
//	unsigned TexColorBuffer;
//	FrameBuffer = CreateFramebuffer();
//	
//
//	glGenTextures(1, &TexColorBuffer);
//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, TexColorBuffer);
//	//注意当前视口的宽度和高度和给这个纹理的值是空的 4是采样点数量
//	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aSampleNum, GL_RGB, aTextureParam.Width, aTextureParam.Height, GL_TRUE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	//绑定纹理附件到帧缓冲，将它附加到GL_COLOR_ATTACHMENT0,最后的0是多级渐远纹理的级别，这里为0
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);
//	//检查帧缓冲是否是完整的，否则打印错误信息
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//删除失败帧缓冲
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout << "[DebugLog] @CC_Common::CreateEmptyMSTexture: ERROR: Framebuffer is not complete!" << std::endl;
//		//如果帧缓冲创建失败，返回默认的帧缓冲对象
//		return TFrameBufferID();
//	}
//	std::cout << "[DebugLog] @CC_Common::CreateEmptyMSTexture: RenderBuffer Created!" << std::endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	TFrameBufferID FrameBufferID;
//	FrameBufferID.FrambufferID = FrameBuffer;
//	FrameBufferID.AttachmenID = TexColorBuffer;
//	return FrameBufferID;
//	 
//}
//
//
//
// CircleEngine::TFrameBufferID CircleEngine::CreateRenderBuffer(TRenderBufferParam aRenderBufferParam)
//{
//	unsigned FrameBuffer;		
//	unsigned int RBO;
//	FrameBuffer = CreateFramebuffer();
//
//	//创建渲染缓冲对象，GL_DEPTH24_STENCIL8是深度值和模板值的精度，4 是采样点数量
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, aRenderBufferParam.Width, aRenderBufferParam.Height);
//	
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);//分配完内存之后可以解绑该缓冲对象
//
//	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//
//	//检查帧缓冲是否是完整的，否则打印错误信息
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//删除失败帧缓冲
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout <<"[DebugLog] @CC_Common::CreateRenderBuffer: ERROR: Framebuffer is not complete!" << std::endl;
//		//如果帧缓冲创建失败，返回默认的帧缓冲对象
//		return TFrameBufferID();
//	}
//		
//	//解绑配置好的帧缓冲，即回到默认帧缓冲
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	std::cout << "[DebugLog] @CC_Common::CreateRenderBuffer: RenderBuffer Created!" << std::endl;
//
//	TFrameBufferID FrameBufferID;
//	FrameBufferID.FrambufferID = FrameBuffer;
//	FrameBufferID.AttachmenID = RBO;
//	return FrameBufferID;
//	 
//}
//
// CircleEngine::TFrameBufferID CircleEngine::CreateMSRenderBuffer(TRenderBufferParam aRenderBufferParam, unsigned aSampleNum)
//{
//	unsigned FrameBuffer;	
//	unsigned int RBO;
//	FrameBuffer = CreateFramebuffer();///生成帧缓冲且绑定
//	glGenRenderbuffers(1, &RBO);
//	 
//
//	//创建渲染缓冲对象，GL_DEPTH24_STENCIL8是深度值和模板值的精度
//	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); // Allocate storage for render buffer object
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_RGB, aRenderBufferParam.Width, aRenderBufferParam.Height);
//
//	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
//	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); // Attach MS render buffer object to framebuffer
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);//分配完内存之后可以解绑该缓冲对象
//	//检查帧缓冲是否是完整的，否则打印错误信息
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//删除失败帧缓冲
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout << "[DebugLog] @CC_Common::CreateMSRenderBuffer: ERROR: Framebuffer is not complete!" << std::endl;
//		//如果帧缓冲创建失败，返回默认的帧缓冲对象
//		return TFrameBufferID();
//	}
//
//	//解绑配置好的帧缓冲，即回到默认帧缓冲
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	std::cout << "[DebugLog] @CC_Common::CreateMSRenderBuffer: RenderBuffer Created!" << std::endl;
//
//	TFrameBufferID FrameBufferID;
//	FrameBufferID.FrambufferID = FrameBuffer;
//	FrameBufferID.AttachmenID = RBO;
//	return FrameBufferID;
//}
//
// 
