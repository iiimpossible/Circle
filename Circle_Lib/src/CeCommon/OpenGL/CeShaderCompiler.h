#pragma once
#include <string>
#include <vector>
#include <CeCommon/Common.h>
using std::vector;
using std::string;


#define INLINESHADER(ShaderCode) #ShaderCode

/**
 * @brief 这个类管理Opengl的着色器编译
 */
class CeShaderCompiler
{
public:
	//编译

	/** 着色器编译*/
	static unsigned CompileShader(const string& aVShader, const string& aFShader, const string& aGShader = "");
	
public:
	//着色器操作

	static void UseShaderProgram(unsigned aShaderHD);
	/** 删除着色器*/
	static void DeleteShaderProgram(unsigned aShaderHD);

	static void DeleteShaderProgram(const vector<unsigned>& aShaderHds);
	/** 实际上是删除原来的Shader然后重新编译一个新的*/
	static unsigned ReCompileShader(unsigned aOldShaderHD, const char* aVShader, 
		const char* aFShader, const char* aGShader = nullptr);

public:
	//着色器赋值

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