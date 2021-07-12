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
1.���������ƬԪ��ɫ������ID��
2.��ȡԴ�ļ�
3.����Դ�ļ�
4.����shader�������Ӵ���
*/

	unsigned vertex, fragment, geometry;
	unsigned programID;
	//������ɫ��
	int success;
	char infoLog[512];


	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShader, nullptr);
	glCompileShader(vertex);

	//������ɫ��������

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "[DebugLog] @ShaderCompile: ERROR:Vertex Shader Compiling Failed!\n" << infoLog << std::endl;
		//EventManager::OnShaderCompileError(0);
	}
	  
	//ƬԪ��ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShadher, nullptr);
	glCompileShader(fragment);

	//ƬԪ��ɫ��������
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "[DebugLog] @ShaderCompile: ERROR:Fragment Shader Compiling Failed!\n" << infoLog << std::endl;
		//EventManager::OnShaderCompileError(0);
	} 

	//�����ɫ������
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
	//����
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
 * @brief ��������Ϣд���Դ�
 * @param vertices ��������
 * @param total_data_size_of_float �����ܹ�����float(���������ܴ�С)
 * @param data_sets_menber_num һ�������ɼ�����Ա���
 * @param data_size_of_float һ�������ܹ�����float(��ά����)
 * @param offset ���ݷֲ����
 * @param layout ������Ϣ��� layout belong to [0,16]
 * @return VAO�������������ID
 * @node ��������float����
 * TODO������������������󣬶��㻺�����ֿ����������ܹ���������������ˣ�
 Ҫ��һ�����㻺��������������ٴΰ󶨾ͺ��ˣ������Ƿ�̬�洢
 */
unsigned CircleEngine::CreateVBO(
	unsigned VAO,
	float* vertices,
	unsigned data_size,//�����ܹ�����float,�������ڴ�
	unsigned vector_demension,//һ��λ���������������꣬����������ά��
	unsigned stride,//ָ��������������֮���ƫ����
	unsigned offset,//����ÿ������
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
* @brief �����������ԡ�
* @param[in] VAO �������������
* @param[in] vertex �������ݼ����ʽ��Ϣ������˳��<0>����ָ�룬<1>�����С��<2>��������ά�ȣ�<3>����
* @param[in] tex_coord �����������鼰����Ϣ��
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

	 //����һ�����������һ���յĶ��㻺��
	 glGenVertexArrays(1, &VBO);
	 glGenBuffers(1, &VBO);
	 glBindVertexArray(aVAO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, DataSize * sizeof(float), nullptr, GL_STATIC_DRAW);

	 //�����㻺����������
	 glBufferSubData(GL_ARRAY_BUFFER, 0, PositionSize * sizeof(float), Position);
	 glBufferSubData(GL_ARRAY_BUFFER, PositionSize * sizeof(float), TextureSize * sizeof(float), Texture);

	 //���߶������鶥�㻺��ĸ�ʽ
	 glVertexAttribPointer(0, PositionDemension, GL_FLOAT, GL_FALSE, PositionStride * sizeof(float), (void*)0);
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(1, TextureDemenSion, GL_FLOAT, GL_FALSE, TextureStride * sizeof(float), (void*)(PositionSize * sizeof(float)));//ע�����������ȡ���ƫ��
	 glEnableVertexAttribArray(1);

	 //��󶥵�����,���㻺��
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

	 //����һ�����������һ���յĶ��㻺��
	 glGenVertexArrays(1, &VBO);
	 glGenBuffers(1, &VBO);
	 glBindVertexArray(aVAO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, DataSize * sizeof(float), nullptr, GL_STATIC_DRAW);

	 //�����㻺����������
	 glBufferSubData(GL_ARRAY_BUFFER, 0, PositionSize * sizeof(float), Position);
	 glBufferSubData(GL_ARRAY_BUFFER, PositionSize * sizeof(float), TextureSize * sizeof(float), Texture);
	 glBufferSubData(GL_ARRAY_BUFFER, (PositionSize + TextureSize) * sizeof(float), NormalSize * sizeof(float), Normal);

	 //���߶������鶥�㻺��ĸ�ʽ
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(0, PositionDemension, GL_FLOAT, GL_FALSE, PositionStride * sizeof(float), (void*)0);
	
	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(1, NormalDemenSion, GL_FLOAT, GL_FALSE, NormalStride * sizeof(float), (void*)((PositionSize + TextureSize) * sizeof(float)));
	
	 glEnableVertexAttribArray(2);
	 glVertexAttribPointer(2, TextureDemenSion, GL_FLOAT, GL_FALSE, TextureStride * sizeof(float), (void*)(PositionSize * sizeof(float)));//ע�����������ȡ���ƫ��
	
	 //��󶥵�����,���㻺��
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);

	 return VBO;
 }

 /*
 �ṹ�������һ���ܺõ���;������Ԥ����ָ��offsetof(s, m)�����ĵ�һ��������һ���ṹ�壬
 �ڶ�������������ṹ���б��������֡������᷵���Ǹ�������ṹ��ͷ�����ֽ�ƫ����(Byte Offset)��
 */
 //unsigned CircleEngine::CreateVBO(unsigned aVAO, const Mesh & aMeshInfo)
 //{

	// unsigned VBO;
	// glGenBuffers(1, &VBO);

	// glBindVertexArray(aVAO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData(GL_ARRAY_BUFFER, aMeshInfo.Vertices.size() * sizeof(Vertex), &aMeshInfo.Vertices[0], GL_STATIC_DRAW);//���static��Ҫ�ĳɿ��Ա仯��
	// 
	// // ����λ��
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// 
	// // ���㷨��
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// // ������������
	// glEnableVertexAttribArray(2);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	// glBindVertexArray(0);

	// return 0;
 //}
/**
 * @brief ����һ���յķ�����ָ���ڴ�ռ��VBO
 * @param total_data_size_of_float �����ܹ�����float(���������ܴ�С)
 * @param data_sets_menber_num һ�������ɼ�����Ա���
 * @param data_size_of_float һ�������ܹ�����float(��ά����)
 * @param offset ���ݷֲ����
 * @param layout ������Ϣ��� layout belong to [0,16]
 * @return VBO�������������ID
 * @note ��������float���飬�������ڶ�̬����洢 
 */
unsigned CircleEngine::CreateVBOEmpty(unsigned VAO, unsigned aDataSizeOfFloat, unsigned data_sets_menber_num, unsigned data_size_of_float, unsigned offset, unsigned layout)
{
	unsigned VBO;

	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO); 

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����ָ����С�Դ棬���ǲ���������
	glBufferData(GL_ARRAY_BUFFER, aDataSizeOfFloat * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	//��ָ����������OpenGL�Ͳ�֪����ô��ȡ����
	glVertexAttribPointer(0, data_sets_menber_num, GL_FLOAT, GL_FALSE, data_size_of_float * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VBO;
}
/**
 * @brief ������������Ϣд���Դ�
 * @param total_data_size_of_float �����ܹ�����float(���������ܴ�С)
 * @param data_sets_menber_num һ�������ɼ�����Ա���
 * @param layout ���������洢��� layout belong to [0,16]
 * @node ��������float����
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
* @brief ����VBO���ݻ�����дһ����
* @param VBO ֮ǰ������VBO
* @param new_vertices �µ�����
* @param start_index ��д��ʼ�㣬Ĭ��Ϊ0����ȫ����д
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
//	{	//�ڶ��������Ƕ༶��Զ������0Ϊ�������𡣳ߴ��ߵ�0����ʷ�������⣬����0ֵ
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
//	//�������������2D�������ͣ�s����t������,�����3D������һ��r,��x,y,z�ȼۡ��ظ�����
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
//	{	//�ڶ��������Ƕ༶��Զ������0Ϊ�������𡣳ߴ��ߵ�0����ʷ�������⣬����0ֵ
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
//	//�������������2D�������ͣ�s����t������,�����3D������һ��r,��x,y,z�ȼۡ��ظ�����
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
//	//ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ͸���������ֵ�ǿյ�
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aWidth, aHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);//������󶨵�֡����
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
//	//������Ⱦ�������GL_DEPTH24_STENCIL8�����ֵ��ģ��ֵ�ľ��ȣ�4 �ǲ���������
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, aWidth, aHeight);
//	//����Ⱦ������󸽼ӵ�֡�������Ⱥ�ģ�帽����
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//	return RBO;
//}
//
///**
//* @brief ��Ϊһ�����ز�������Ⱦ������󣬰󶨵�֡����GL_COLOR_ATTACHMENT0�����ڴ�ΪGL_RGB��ʽ
//*/
//unsigned CircleEngine::CreateMSRenderBuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum)
//{
//	unsigned int RBO;
//
//	//������Ⱦ�������GL_DEPTH24_STENCIL8�����ֵ��ģ��ֵ�ľ���
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_RGB, aWidth, aHeight);
//	//����Ⱦ������󸽼ӵ�֡�������Ⱥ�ģ�帽����
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
//	// ��������
//	unsigned FrameBuffer;
//	FrameBuffer = CreateFramebuffer();
//	unsigned TexColorBuffer;
//	glGenTextures(1, &TexColorBuffer);
//	glBindTexture(GL_TEXTURE_2D, TexColorBuffer);
//	//ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ͸���������ֵ�ǿյ�
//	glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, aTextureParam.Width, aTextureParam.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	//����������֡���壬�������ӵ�GL_COLOR_ATTACHMENT0,����0�Ƕ༶��Զ����ļ�������Ϊ0
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);
//	//���֡�����Ƿ��������ģ������ӡ������Ϣ
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//ɾ��ʧ��֡����
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout << "[DebugLog] @CC_Common::CreateTextureBuffer: ERROR: Framebuffer is not complete!" << std::endl;
//		//���֡���崴��ʧ�ܣ�����Ĭ�ϵ�֡�������
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
//	 //ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ�format = depth component, type = float
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
// //����һ�����ж��ز����������Ķ��ز���֡����֡���壬 
// CircleEngine::TFrameBufferID CircleEngine::CreateMSColorbuffer(TTextureParam aTextureParam, unsigned aSampleNum)
//{
//	unsigned FrameBuffer;	 
//	unsigned TexColorBuffer;
//	FrameBuffer = CreateFramebuffer();
//	
//
//	glGenTextures(1, &TexColorBuffer);
//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, TexColorBuffer);
//	//ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ͸���������ֵ�ǿյ� 4�ǲ���������
//	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aSampleNum, GL_RGB, aTextureParam.Width, aTextureParam.Height, GL_TRUE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	//����������֡���壬�������ӵ�GL_COLOR_ATTACHMENT0,����0�Ƕ༶��Զ����ļ�������Ϊ0
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);
//	//���֡�����Ƿ��������ģ������ӡ������Ϣ
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//ɾ��ʧ��֡����
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout << "[DebugLog] @CC_Common::CreateEmptyMSTexture: ERROR: Framebuffer is not complete!" << std::endl;
//		//���֡���崴��ʧ�ܣ�����Ĭ�ϵ�֡�������
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
//	//������Ⱦ�������GL_DEPTH24_STENCIL8�����ֵ��ģ��ֵ�ľ��ȣ�4 �ǲ���������
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, aRenderBufferParam.Width, aRenderBufferParam.Height);
//	
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);//�������ڴ�֮����Խ��û������
//
//	//����Ⱦ������󸽼ӵ�֡�������Ⱥ�ģ�帽����
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//
//	//���֡�����Ƿ��������ģ������ӡ������Ϣ
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//ɾ��ʧ��֡����
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout <<"[DebugLog] @CC_Common::CreateRenderBuffer: ERROR: Framebuffer is not complete!" << std::endl;
//		//���֡���崴��ʧ�ܣ�����Ĭ�ϵ�֡�������
//		return TFrameBufferID();
//	}
//		
//	//������úõ�֡���壬���ص�Ĭ��֡����
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
//	FrameBuffer = CreateFramebuffer();///����֡�����Ұ�
//	glGenRenderbuffers(1, &RBO);
//	 
//
//	//������Ⱦ�������GL_DEPTH24_STENCIL8�����ֵ��ģ��ֵ�ľ���
//	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); // Allocate storage for render buffer object
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_RGB, aRenderBufferParam.Width, aRenderBufferParam.Height);
//
//	//����Ⱦ������󸽼ӵ�֡�������Ⱥ�ģ�帽����
//	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); // Attach MS render buffer object to framebuffer
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);//�������ڴ�֮����Խ��û������
//	//���֡�����Ƿ��������ģ������ӡ������Ϣ
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//ɾ��ʧ��֡����
//		glDeleteFramebuffers(1, &FrameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		std::cout << "[DebugLog] @CC_Common::CreateMSRenderBuffer: ERROR: Framebuffer is not complete!" << std::endl;
//		//���֡���崴��ʧ�ܣ�����Ĭ�ϵ�֡�������
//		return TFrameBufferID();
//	}
//
//	//������úõ�֡���壬���ص�Ĭ��֡����
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
