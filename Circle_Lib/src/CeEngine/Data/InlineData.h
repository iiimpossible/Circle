#pragma once
#include <vector>
using std::vector;



/**
 * @brief ������������
 */
class InlineMeshData
{
public:
	static vector<float> Cube_Vertices;
	static vector<float> Cube_Normals;
	static vector<float> Cube_TexCoords;
	static vector<unsigned> Cube_Indices;
	static vector<float> FullSreenRecVertices;
	static vector<float> FullSreenRecTexcoods;
};

#define INLINESHADER(ShaderCode) #ShaderCode
/**
 * @brief ���ô�ɫ��ɫ��
 */
class InlineShaderCode
{
public:
	//static const char* s;
	static const char* PureColor_VS3D;
	static const char* PureColor_FS3D;

	static const char* PureColor_VS2D;
	static const char* PureColor_FS2D;

	static const char* Line_VS;
	static const char* Line_FS;

	static const char* Rec_VS;
	static const char* Rec_FS;

	static const char* Skybox_VS;
	static const char* Skybox_FS;

};
//��ɫ��ɫ��3D

class FullSreenQuad 
{
public:
	FullSreenQuad();
	~FullSreenQuad();
public:

	inline const int GetVAO() const { return mVAO; };

	inline const int GetVBO() const { return mVBO; };

	inline unsigned GetVertexiceNum() { return 6; }
private:

	unsigned mVAO;
	unsigned  mVBO;

};
//��ɫ��ɫ��2D
