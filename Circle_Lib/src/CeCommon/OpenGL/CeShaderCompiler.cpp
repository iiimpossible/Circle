#define GLEW_STATIC
#include <GL/glew.h>
//#include <glad/glad.h>
#include <iostream>
#include "CeShaderCompiler.h"
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>

unsigned CeShaderCompiler::CompileShader(const string& aVertex, const string& aFragment, const string& aGeometry)
{
	if (aVertex.empty() || aFragment.empty())
	{
		DebugLogManager::DebugOutput("CeShaderCompiler", EDebugTag::EDT_ERROR,{ "Vertex or fragment shader is null." });
		return 0;
	}
	const char* vshader = aVertex.c_str();
	const char* fshader = aFragment.c_str();
	const char* gshader = aGeometry.c_str();

	unsigned vertex, fragment, geometry;
	unsigned program_id;
	//创建着色器
	int success;
	char infoLog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vshader, nullptr);
	glCompileShader(vertex);

	//顶点着色器错误处理
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		DebugLogManager::DebugOutput("CeShaderCompiler", EDebugTag::EDT_ERROR, {"\n",infoLog ,vshader});
		return 0;
	}

	//片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fshader, nullptr);
	glCompileShader(fragment);

	//片元着色器错误处理
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		DebugLogManager::DebugOutput("CeShaderCompiler", EDebugTag::EDT_ERROR,{ "\n",infoLog,fshader });
		return 0;
	}

	//获得着色器把手
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex);
	glAttachShader(program_id, fragment);

	if (!aGeometry.empty())
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gshader, nullptr);
		glCompileShader(geometry);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
			DebugLogManager::DebugOutput("CeShaderCompiler", EDebugTag::EDT_ERROR, { "\n", infoLog, gshader });
			return 0;
		}
		glAttachShader(program_id, geometry);
	}
	//链接
	glLinkProgram(program_id);

	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program_id, 512, NULL, infoLog);
		DebugLogManager::DebugOutput("CeShaderCompiler", EDebugTag::EDT_ERROR, { "\n",infoLog});
		return 0;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!aGeometry.empty())
	{
		glDeleteShader(geometry);
	}

	return program_id;
}

void CeShaderCompiler::DeleteShaderProgram(unsigned aShaderHD)
{
	glDeleteProgram(aShaderHD);
}

void CeShaderCompiler::DeleteShaderProgram(const vector<unsigned>& aShaderHds)
{
	for (auto val : aShaderHds)
	{
		glDeleteProgram(val);
	}
}

unsigned CeShaderCompiler::ReCompileShader(unsigned aOldShaderHD, const char* aVShader, const char * aFShader, const char * aGShader)
{
	unsigned tShaderHD = CompileShader(aVShader, aFShader, aGShader);
	glDeleteProgram(aOldShaderHD);
	return tShaderHD;
}

void CeShaderCompiler::UseShaderProgram(unsigned aShaderHD)
{
	glUseProgram(aShaderHD);	
}


void CeShaderCompiler::SetBool(const unsigned int shaderId, const string &name, bool value)
{
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), (int)value);
}

// ------------------------------------------------------------------------
void CeShaderCompiler::SetInt(const unsigned int shaderId, const string &name, int value)
{
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}
void CeShaderCompiler::SetInt(const unsigned int aShaderID, const string & aName, unsigned aElmentNum, int* aValue)
{
	glUniform1iv(glGetUniformLocation(aShaderID, aName.c_str()), aElmentNum, aValue);
}
// ------------------------------------------------------------------------
void CeShaderCompiler::SetFloat(const unsigned int shaderId, const string &name, float value)
{
	glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void CeShaderCompiler::SetFloat(const unsigned int aShaderID, const string & aName, unsigned aElmentNum, float* aValue)
{
	glUniform1fv(glGetUniformLocation(aShaderID, aName.c_str()), aElmentNum, aValue);
}

void CeShaderCompiler::SetVec2(const unsigned int aShaderID, const string & aName, const Vec2 & aValue)
{
	glUniform2fv(glGetUniformLocation(aShaderID, aName.c_str()), 1, &aValue[0]);
}


void CeShaderCompiler::SetVec2(const unsigned int aShaderID, const string & aName, unsigned aElementNum, const Vec2 & aValue)
{
	glUniform2fv(glGetUniformLocation(aShaderID, aName.c_str()), aElementNum, &aValue[0]);
}

void CeShaderCompiler::SetVec3(const unsigned int shaderId, const string &name, const Vec3 &value)
{
	glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
}

void CeShaderCompiler::SetVec4(const unsigned int shaderId, const string &name, const Vec4 &value)
{
	glUniform4fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
}

void CeShaderCompiler::SetMat2(const unsigned int shaderId, const string &name, const Mat2 &mat)
{
	glUniformMatrix2fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CeShaderCompiler::SetMat3(const unsigned int shaderId, const string &name, const Mat3 &mat)
{
	glUniformMatrix3fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CeShaderCompiler::SetMat4(const unsigned int shaderId, const string &name, const Mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned CeShaderCompiler::SetTextureUnitName(const unsigned int aShaderID, const string & aName, unsigned aTexUnit)
{
	glUniform1i(glGetUniformLocation(aShaderID, aName.c_str()), aTexUnit);
	return aTexUnit + 1;
}

void CeShaderCompiler::SetTextureUnitName(const unsigned int aShaderID, const vector<string>& aName, unsigned aTexUnitOffset)
{
	for (int i = 0; i < aName.size() && i < 15 - aTexUnitOffset; i++)
	{
		glUniform1i(glGetUniformLocation(aShaderID, aName[i].c_str()), i + aTexUnitOffset);
	}

}
