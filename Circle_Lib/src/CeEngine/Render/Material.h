#pragma once
#include <vector>
#include <string>
#include <Celib_x64/src/CeCommon/Common.h>
using std::string;
using std::vector;
using CircleEngine::Vec3;


/**
* @brief 材质是描述物体表面光学特性的一组数据的集合
* @detail 材质输入已经载入显存的纹理句柄和着色器，在初始化的时候，将纹理句柄和着色器绑定
* 材质文件实际上是一个简单的脚本文件，需要写一个特定的词法分析器以将其中的信息提取出来
* TODO：支持多纹理,支持离屏渲染
* 在这里创建一个多重采样帧缓冲，和一个中介帧缓冲，
* 将渲染到多重采样帧缓冲的颜色数据复制到中介帧缓冲，
* 将中介帧缓冲作为纹理

* 以后实现：一个属性列表，根据这个属性列表的命令来配置着色器变量，即支持简单材质编辑
*/
class Material
{
public:
	Material(bool aIsLight = false);
	Material(const vector<ShaderElement>& aShader);
	//Material(const Material& aMaterial);
	//void operator=(const Material& aMaterial);
	~Material();
public:
	//光照相关

	inline const Vec3& GetEmissiveColor() { return mLightColor; }//函数后c

	void SetEmissiveColor(const Vec3& aColor);

	void SetEmissiveColor(float aR, float aG, float aB);


public:
	//着色器和纹理句柄

	/** 最新接口，获得着色器句柄数组*/
	inline vector<unsigned> GetShaderHds() const { return mShaderHds; }

	/** 将要删除*/
	inline vector<unsigned> GetTextureHds() const { return mTexture2DHds; }

	inline vector<ShaderElement> GetShaderElements() const { return mShaderElemets; }

	inline string GetPath() const { return mPath; }

public:

	inline void SetShaderHDs(const vector<unsigned>& aShaderHDs) { mShaderHds = aShaderHDs; }

	inline void SetShaderElements(const vector<ShaderElement>& aShaderElements) { mShaderElemets = aShaderElements; }

	inline void SetPath(const string& aPath) { mPath = aPath; }

	inline void Clear() { mShaderElemets.clear(); mTexture2DHds.clear(); mShaderHds.clear(); }
public:
	//以后的可编辑材质

	/** 将纹理句柄存储是为了删除无用的纹理，因为这些纹理句柄不再存储在资源管理器中了*/
	inline void AddTexture(unsigned aTextureHandle) { mTexture2DHds.push_back(aTextureHandle); }

	void AddShaderElement(unsigned aShaderHd, const vector<unsigned>& aTexHds, const vector<string>& aTexNames);
	 
	inline void AddTexture(const vector<unsigned> aTexHDs) { mTexture2DHds = aTexHDs; }

	inline void AddShader(unsigned aShaderHD) { mShaderHds.push_back(aShaderHD); }
	

	inline void SetShaderConfig(const string& aValue) { mShaderConfig.push_back(aValue); }

private:
	//光照属性  是否泛光，泛光强度 阴影 反射 折射 
	Vec3 mLightColor;
private:	
	//网格和着色器

	vector<string> mShaderConfig;

	//存储所有的纹理句柄，按照读取顺序存储，还是使用一个map？map对性能有损耗吗？
	vector<unsigned> mTexture2DHds;

	vector<unsigned> mShaderHds;	

	vector<ShaderElement> mShaderElemets;

	string mPath;
	
	bool bIsLight;
};

 