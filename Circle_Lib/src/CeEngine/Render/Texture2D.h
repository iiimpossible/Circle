#pragma once
#include <iostream>
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
#include <Celib_x64/src/CeCommon/OpenGL/CeTextureFomatter.h>
using std::string;




 /**
 * @brief 基本数据类型，存储一个2D纹理句柄
 * @details 生成纹理并保存改纹理的ID
 */
class Texture2D 
{
public:	
	Texture2D(unsigned aTexHD = 0, int aWidth = 0, int aHeight = 0, int aChannels = 3);
	Texture2D(const Texture2DInfo& aInfo);
	//TODO:纹理选项，过滤，环绕方式，一个枚举
	~Texture2D();
	
public:
	//纹理句柄

	/** 获得纹理句柄*/
	inline unsigned GetTexture2DHandle() const { return mTex2DHD; }
	
public:
	//纹理属性获取

	inline int GetWidth() const { return mWidth; };
	inline int GetHight() const { return mHeight; }
	inline int GetChannels() const { return mChannel; }

	inline const string& GetTypeName() { return mTypeName; }
	inline const string& GetPath() { return mPath; }
private:
	unsigned mTex2DHD;
	int mWidth; 
	int mHeight;
	int mChannel;
	string mTypeName;//在shader中规定uniform纹理采样变量的名字以texture_diffuse1的形式命名
	string mPath;//这是加载后，从assimp中获得的纹理路径，用来和新加载的纹理比对，如果相同就意味着该纹理已经加载
	//bool m_InternalFormat;
	//bool m_ImageFormat;
};
