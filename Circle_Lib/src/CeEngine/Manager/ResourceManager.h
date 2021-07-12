#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
class Texture2D;
class Shader;
class StaticMesh;
class Material;

//class Character;
class ResourceManager
{
public:	
	ResourceManager();
	~ResourceManager();	

public:
	//初始化
	void Start();


public:
	//加载

	/** 解析材质并加载相应资源*/
	static int LoadMaterialOld(const string& aPath);

	static int LoadMaterial(const string & aPath, const string& aName = "");

	/** 加载纹理只提供句柄（给材质）*/
	static unsigned LoadTexture2D(const string& aPath);

	static unsigned LoadTextureCube(const string& aPath);

	/** 加载图片并输入为Texture，并用纹理名字存在map中*/
	static Texture2D LoadTexture(const char* file, string aName);

	/** 从文本文件中加载着色器，这个函数不再需要了*/
	static Shader LoadShader(const char* aVShader, const char*aFShader, const char* aGShader, string aName) = delete;

	/** 加载模型并用名字储存在map中*/
	static int LoadStaticMesh(const string& aPath);

	 
public:
	//获得

	static Material& GetMaterial(const string& aName);	
	/** 从容器中返回该纹理引用*/
	static Texture2D& GetTexture(string aName);	
	/** 从容器中返回该着色器引用*/
	static Shader& GetShader(string aName);	

	static StaticMesh& GetStaticMesh(const string& aName);	

	static vector<string> GetMaterialList();
public:
	//查找

	static bool FindMaterial(const string& aName);
private:
	//

	/** 材质重加载。现在是暴力删除，之后应该先检查修改了哪些内容，然后再考虑是否删除纹理和着色器*/
	static int ReloadMaterial(const string& aName, const string& aPath);
private:
	///纹理容器
	static map<string, Texture2D> mTextures;
	///着色器容器
	static map<string, Shader> mShaders;

	static map<string, StaticMesh> mStaticMeshes;

	static unordered_map<string, Material> mMaterials;

	static unordered_map<string, unsigned> mTextureCubes;
	///
	//static map<unsigned char, Character> characters;
	/**
	 * @brief TODO:者两个函数要独立出去，成为一个获取数据的类
	 */
	static Texture2D LoadTextureFromFile(const char* file);
	
	
};