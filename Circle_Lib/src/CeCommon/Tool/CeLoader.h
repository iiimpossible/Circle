#pragma once
#include <vector>
#include <string>
#include <Celib_x64/src/CeCommon/Math/Math.h>
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
using std::vector;
using std::string;

/**
 * @brief 这个类是管理所有文件加载的类，包括文本、图像、模型
 * @detail 这个加载是指加载文本来编译着色器，加载图像到显存，加载模型到显存
 * 未来还包括定义和读取自定义的文件，如地图文件、材质文件
 * @note 注意文件包含路径相对起点为头文件本身，资源文件相对路径起点为工程文件目录
 */
class CeLoader
{	
public:
	//模型加载

	/**
	 * @brief 使用assimp库加载模型，只加载网格不加载纹理，纹理使用材质中定义的路径加载
	 * @param aPath 模型路径
	 */
	static vector<CeMesh>& LoadModel(const string& aPath);

public:
	//图片加载

	/**
	 * @brief 纹理加载函数
	 * @param aPath 纹理路径
	 * @param aWidth 纹理X轴向像素数目，指针输出
	 * @param aheight 纹理Y轴向像素数目，指针输出
	 * @param aChannel 纹理通道数目，指针输出
	 * @param aReqComp 忽略
	 * @note 该方法返回的unsigned char* 是一块堆内存，注意释放
	 */	 
	static unsigned char* LoadImageData(const char * aPath, int* aWidth, int* aheight, int* aChannel, int aReqComp = 0);

	/**
	 * @breif  加载天空盒子
	 * @note 天空盒子纹理有命名规范，一定要是输入的路径后边加上【下划线+方位】，如第一个后缀【_right.jpg】
	 * 输入的路径后缀不需要加，该函数会自动添加	 
	 * @detail 纹理目标	方位
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_X	右
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_X	左
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Y	上
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	下
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Z	后
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	前
	 */	 
	static vector<unsigned char*> LoadCubeImageData(const string& aPath, int* aWidth, int* aHeight, int* aChannel, int aReqComp = 0);

	/** 将数据输入输入后可以释放内存*/
	static void FreeImageData(unsigned char * aData);

	/** @breif 释放天空盒子的加载数据*/
	static void FreeCubeImageData(vector<unsigned char*> aData);

public:
	//文本加载

	static string LoadText(const string& aPath);

	static string GetFileNameFromPath(const string& aPath);
public:

	/**
	 * @brief 加载材质文件并提取出其中的信息
	 * @param aPaht 材质文件路径
	 * @param aTexName 纹理名字数组，指针输出
	 * @param aTexPath 纹理路径，指针输出
	 * @param aShaderCode 着色器代码，指针输出
	 */
	static void LoadMaterial(const char* aPath, 
		vector<string>* aTexName, 
		vector<string>* aTexPath, 
		vector<string>* aShaderCode);


private:

	/** @brief 将Assimp节点存储的数据输出为一个单独的网格*/
	static CeMesh ProcessMesh(struct aiMesh* aMehs, const struct aiScene* aScene);

	/** @brief 使用加载纹理的函数从模型中读取纹理*/
	//static void LoadMaterialTextures(struct aiMaterial *mat, enum aiTextureType type, const char* typeName);

	/** @brief 处理Assimp的节点数据类型，递归形式地处理*/
	static void ProcessNode(struct aiNode* aNode, const struct  aiScene* aScene);

	/** 每次加载前会清理*/
	static vector<CeMesh> mTempMesh;
};



//CELOADX64_API void LoadModel(const char* aPath);

// C helper functions:
//
//static char**makeCharArray(int size) {
//	return new(sizeof(char*), size);
//}
//
//static void setArrayString(char **a, char *s, int n) {
//	a[n] = s;
//}
//
//static void freeCharArray(char **a, int size) {
//	int i;
//	for (i = 0; i < size; i++)
//		free(a[i]);
//	free(a);
//}
//
//// Build C array in Go from sargs []string
//
//cargs: = C.makeCharArray(C.int(len(sargs)))
//	defer C.freeCharArray(cargs, C.int(len(sargs)))
//	for i, s : = range sargs{
//			C.setArrayString(cargs, C.CString(s), C.int(i))
//	}