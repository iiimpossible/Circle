#include "Sprite.h"
#include <CeCommon/OpenGL/CeShaderCompiler.h>
using namespace CircleEngine;
void Sprite::SetShader(const Shader& aShader)
{
	mShader = aShader;
}
void Sprite::SetTexture2D(const Texture2D& aTex2D)
{
	mTexture2D = aTex2D;
}


void Sprite::SetRank(float aRow, float aCloumn)
{
	int tHandle = mShader.GetShaderHandle();
	CeShaderCompiler::UseShaderProgram(tHandle);
	CeShaderCompiler::SetBool(tHandle,"iSFAnim",true);
	CeShaderCompiler::SetFloat(tHandle,"iRow",aRow);
	CeShaderCompiler::SetFloat(tHandle,"iColumn", aCloumn);

	//应该放到构造函数里边的。
	SetTimeSpeed();
}

void Sprite::SetTimeSpeed(float aSpeed)
{
	int tHandle = mShader.GetShaderHandle();
	CeShaderCompiler::UseShaderProgram(tHandle);
	CeShaderCompiler::SetFloat(tHandle, "iSpeed", aSpeed);
}

const Texture2D& Sprite::GetTexture2D() const
{
	return mTexture2D;
}
const Shader& Sprite::GetShader()const
{
	return mShader;
}