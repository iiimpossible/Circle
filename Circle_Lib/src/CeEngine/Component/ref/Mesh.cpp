#include "Mesh.h"
//#include <qdebug.h>
#include  <GL/glew.h>
#include "../../BOCommon/Common.h"
using namespace BOCommon;
Mesh::Mesh()
{
	
	
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
	bm_drawByIndex = true;
	this->vertices = vertices;
	this->indices = indices;


	setupMesh();
}

Mesh::~Mesh()
{

}

unsigned int Mesh::GetVAOID() const
{
	return this->m_VAOID;
}

unsigned int Mesh::GetVBOID() const
{
	return this->m_VBOID;
}

unsigned int Mesh::GetEBOID() const
{
	return this->m_EBOID;
}

Mesh::Mesh(float *vertex, int size)
{
	this->vertices = ArrayToVector(vertex, size);
	this->bm_drawByIndex = false;
	this->bm_hasTangent = false;
	SetupMesh();
}
vector<Vertex> Mesh::ArrayToVector(float* vertices, int size)
{
	if (!vertices)
	{
		DebugLog("ERROE:ARRAT TO VERTICES PTR! CLASS: MESH");
	}
	if (size % 8 != 0)
	{
		DebugLog("ERROE:SIZE INCRRECTNESS!")
	}
	vector<Vertex> b;
	vector<float> sss;
	Vertex temp_vertex;
	for (int i = 0; i < size; i++)
	{
		sss.push_back(vertices[i]);
		if (sss.size() >= 8)
		{
			temp_vertex.Position.x = sss[0];
			temp_vertex.Position.y = sss[1];
			temp_vertex.Position.z = sss[2];
			temp_vertex.Normal.x = sss[3];
			temp_vertex.Normal.y = sss[4];
			temp_vertex.Normal.z = sss[5];
			temp_vertex.TexCoords.x = sss[6];
			temp_vertex.TexCoords.y = sss[7];
			sss.clear();
			b.push_back(temp_vertex);
		}
	}
	return b;
}

vector<Vertex> Mesh::GetVertices()const
{
	return this->vertices;
}


void  Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAOID);//创建顶点数组对象，格式化存储顶点
	glGenBuffers(1, &m_VBOID);//创建顶点缓冲对象，顶点数据初步存储在这里
	glBindVertexArray(m_VAOID);	//绑定顶点数组对象
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);//绑定顶点缓冲对象
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);//输入顶点数据
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glBindVertexArray(0);//解绑顶点数组对象
}

void Mesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &m_VAOID);
	glGenBuffers(1, &m_VBOID);

	

	if (bm_drawByIndex)
	{
		glGenBuffers(1, &m_EBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}
	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	if (bm_hasTangent)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}
	Debug(this->m_VAOID);


	glBindVertexArray(0);
}