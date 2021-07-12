#include "Material.h"
#include "../Data/InlineData.h"
#include <Celib_x64/src/CeCommon/OpenGL/CeShaderCompiler.h>
#include <Celib_x64/src/CeCommon/OpenGL/CeTextureFomatter.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
using namespace CircleEngine; 

Material::Material( bool aIsLight) :
	 bIsLight(aIsLight)
{	
}

Material::Material(const vector<ShaderElement>& aShader)
{
	//将纹理绑定到着色器，并初始化一些光学参数（漫反射、高光）

	for (auto val : aShader)
	{
		//CeShaderCompiler::UseShaderProgram(val.shader_hd);
		/*for (int i = 0; i < val.texture_hd.size() && i < val.texture_name.size(); i++)
		{
			CeTextureFormatter::BindTexture2D(val.shader_hd,val.texture_hd[i],val.texture_name[i].c_str(),i);
		}*/
	}
}

Material::~Material(){}

void Material::SetEmissiveColor(float aR, float aG, float aB)
{
	mLightColor.x = aR; mLightColor.y = aG; mLightColor.z = aB;
	for(auto val: mShaderElemets)
	{
		CeShaderCompiler::UseShaderProgram(val.shader_hd);
		Vec3 color(aR, aG, aB);
		CeShaderCompiler::SetVec3(val.shader_hd,"iEmissive",color);
	}	 
}




void Material::SetEmissiveColor(const Vec3& aColor)
{
	mLightColor = aColor; 
	for (auto val : mShaderElemets)
	{
		CeShaderCompiler::UseShaderProgram(val.shader_hd);		 ;
		CeShaderCompiler::SetVec3(val.shader_hd, "iEmissive", aColor);
	}
}


void Material::AddShaderElement(unsigned aShaderHd, const vector<unsigned>& aTexHds, const vector<string>& aTexNames)
{
	ShaderElement se;
	se.shader_hd = aShaderHd;
	se.texture_hd = aTexHds;
	se.texture_name = aTexNames;
	mShaderElemets.push_back(se);
}



