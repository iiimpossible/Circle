#pragma once
#include "Component.h"

struct Vertex;
struct SubMesh;
 /**
 * @brief 一个游戏物体的网格
 * @details 存储模型顶点的类，将输入的顶点信息传入到显存中
 */
class Mesh
{
public:
	
	Mesh();
	/**
	 * @brief 这个构造函数是用于从模型中读取顶点数据
	 */
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
	/**
	 * @brief 这个构造函数用于手动输入数据，注意顶点结构体内部有切向量预留空间，
	 * 如果测试的时候不需要这个就浪费内存
	 */
	Mesh(float *vertex, int size);

	~Mesh();
	/**
	 * @brief 返回VAO句柄
	 */	
	unsigned int GetVAOID() const;
	/**
	 * @brief 返回VBO句柄，其实VBO没什么用
	 */
	unsigned int GetVBOID() const;
	/**
	 * @brief 返回EBO句柄
	 */
	unsigned int GetEBOID() const;
	/**
	 * @brief 获得该网格的顶点
	 */
	vector<Vertex> GetVertices()const;
	/**
	 * @brief 将一组顶点数据转换为标准的顶点
	 */
	vector<Vertex>  ArrayToVector(float * vertices, int size);
private:
	//顶点数组
	vector<Vertex>       vertices;
	//顶点索引数组
	vector<unsigned int> indices;
	////纹理
	////TODO::将纹理移到材质去
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
