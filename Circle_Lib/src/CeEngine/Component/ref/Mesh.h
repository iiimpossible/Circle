#pragma once
#include "Component.h"

struct Vertex;
struct SubMesh;
 /**
 * @brief һ����Ϸ���������
 * @details �洢ģ�Ͷ�����࣬������Ķ�����Ϣ���뵽�Դ���
 */
class Mesh
{
public:
	
	Mesh();
	/**
	 * @brief ������캯�������ڴ�ģ���ж�ȡ��������
	 */
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
	/**
	 * @brief ������캯�������ֶ��������ݣ�ע�ⶥ��ṹ���ڲ���������Ԥ���ռ䣬
	 * ������Ե�ʱ����Ҫ������˷��ڴ�
	 */
	Mesh(float *vertex, int size);

	~Mesh();
	/**
	 * @brief ����VAO���
	 */	
	unsigned int GetVAOID() const;
	/**
	 * @brief ����VBO�������ʵVBOûʲô��
	 */
	unsigned int GetVBOID() const;
	/**
	 * @brief ����EBO���
	 */
	unsigned int GetEBOID() const;
	/**
	 * @brief ��ø�����Ķ���
	 */
	vector<Vertex> GetVertices()const;
	/**
	 * @brief ��һ�鶥������ת��Ϊ��׼�Ķ���
	 */
	vector<Vertex>  ArrayToVector(float * vertices, int size);
private:
	//��������
	vector<Vertex>       vertices;
	//������������
	vector<unsigned int> indices;
	////����
	////TODO::�������Ƶ�����ȥ
	//vector<Texture2D>    textures;
	void setupMesh();
	void SetupMesh();
	unsigned int m_VAOID;
	unsigned int m_VBOID;
	unsigned int m_EBOID;
	bool bm_drawByIndex;
	bool bm_hasTangent;
};




struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};
struct SubMesh
{
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture2D>      textures;
	unsigned int VAO;
};
