#define GLEW_STATIC
#include <GL/glew.h>
//#include <glad/glad.h>
#include "CeMeshFormatter.h"


unsigned CeMeshFormatter::FormatMesh(vector<float> aData)
{
	return 0;
}

unsigned CeMeshFormatter::CreateVAO()
{
	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	
	return VAO;
}

unsigned CeMeshFormatter::CreateEBO(unsigned aVAO, EBOFormat aFormat, EIndiceDrawMode aDrawMode)
{
	unsigned* first = std::get<0>(aFormat);
	unsigned array_size = std::get<1>(aFormat);
	unsigned ebo;
	glBindVertexArray(aVAO);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, array_size * sizeof(float), first, aDrawMode);
	glBindVertexArray(0);
	return ebo;	
}




unsigned CeMeshFormatter::CreateVBO(unsigned aVAO, VerterxFormat aVertexInfo, VerterxFormat aTexCoordInfo)
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

unsigned CeMeshFormatter::CreateVBO(unsigned aVAO, VBODataFormat aVerFormat, VBODataFormat aTexFormat)
{

	unsigned VBO;

	float* Position = aVerFormat.Array;
	unsigned PositionSize = aVerFormat.ArraySize;
	unsigned PositionDemension = aVerFormat.DataDimension;
	unsigned PositionStride = aVerFormat.Stride;

	float* Texture = aTexFormat.Array;
	unsigned TextureSize = aTexFormat.ArraySize;
	unsigned TextureDemenSion = aTexFormat.DataDimension;
	unsigned TextureStride = aTexFormat.Stride;

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


	return 0;
}

unsigned CeMeshFormatter::CreateVBO(unsigned aVAO, VerterxFormat aVertexInfo, VerterxFormat aTexCoordInfo, VerterxFormat aNormalInfo)
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
