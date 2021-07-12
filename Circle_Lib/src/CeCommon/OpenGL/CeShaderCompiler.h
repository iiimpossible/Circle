#pragma once
#include <string>
#include <vector>
#include <CeCommon/Common.h>
using std::vector;
using std::string;


#define INLINESHADER(ShaderCode) #ShaderCode

/**
 * @brief ��������Opengl����ɫ������
 */
class CeShaderCompiler
{
public:
	//����

	/** ��ɫ������*/
	static unsigned CompileShader(const string& aVShader, const string& aFShader, const string& aGShader = "");
	
public:
	//��ɫ������

	static void UseShaderProgram(unsigned aShaderHD);
	/** ɾ����ɫ��*/
	static void DeleteShaderProgram(unsigned aShaderHD);

	static void DeleteShaderProgram(const vector<unsigned>& aShaderHds);
	/** ʵ������ɾ��ԭ����ShaderȻ�����±���һ���µ�*/
	static unsigned ReCompileShader(unsigned aOldShaderHD, const char* aVShader, 
		const char* aFShader, const char* aGShader = nullptr);

public:
	//��ɫ����ֵ

	static void SetBool(const unsigned int aShaderID, const string& aName, bool aValue);

	static void SetInt(const unsigned int aShaderID, const string& aName, int aValue);
	static void SetInt(const unsigned int aShaderID, const string& aName, unsigned aElmentNum, int* aValue);

	static void SetFloat(const unsigned int aShaderID, const string& aName, float aValue);
	static void SetFloat(const unsigned int aShaderID, const string& aName, unsigned aElmentNum, float* aValue);

	static void SetVec2(const unsigned int aShaderID, const string & aName, const Vec2 & aValue);
	static void SetVec2(const unsigned int aShaderID, const string & aName, unsigned aElementNum, const Vec2 & aValue);

	//static void SetVec2(const unsigned int shader_id, const string &aName, float x, float y);
	static void SetVec3(const unsigned int aShaderID, const string& aName, const Vec3& aVctor);
	//static void setVec3(const unsigned int aShaderID, const string &aName, float x, float y, float z);
	static void SetVec4(const unsigned int aShaderID, const string& aName, const Vec4& aVctor);
	//static void SetVec4(const unsigned int aShaderID, const string &aName, float x, float y, float z, float w);
	static void SetMat2(const unsigned int aShaderID, const string& aName, const Mat2& aMat);
	static void SetMat3(const unsigned int aShaderID, const string& aName, const Mat3& aMat);
	static void SetMat4(const unsigned int aShaderID, const string& aName, const Mat4& aMat);

	static unsigned SetTextureUnitName(const unsigned int aShaderID,const string& aName, unsigned aTexUnit = 0 );
	static void SetTextureUnitName(const unsigned int aShaderID, const vector<string>& aName, unsigned aTexUnitOffset = 0);


};