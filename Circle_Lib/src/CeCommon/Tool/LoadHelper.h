
#pragma once
#include <iostream>
#include <string>
#include <vector>
//#include <assimp/material.h>
#include "../Common.h"
#include "../../CCEngine/Render/StaticMesh.h"
using std::string;
using std::vector;
using CircleEngine::Vec3;
using CircleEngine::Vec2;
enum aiTextureType;
//class aiTextureType;

#pragma region CLASS_MODELLOADER

	enum class ETextureType
	{
		ETT_Diffuse,
		ETT_Specular,
		ETT_Ambient
	};

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();
		void InitAssimp();

		//static StaticMesh LoadModel(const string& aPath);

		/**
		* @brief 加载模型函数
		* 通过输入路径调用Assimp库的函数来加载模型，并转换为OpenGL可以识别的数据
		* @param aPath 模型的路径
		* @return StaticMesh 输出为一个静态的网格。该静态网格可能包含了多个子网格
		*/
		static StaticMesh LoadModel(const string& aPath);		

		static void TestDraw(const StaticMesh& aTestMesh);
	private:
		static vector<Mesh> mTempMesh;
		static Mesh mtMesh;
		static vector<Texture> textures_loaded;
		static string directory;


		static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

		/**
		* @brief 将Assimp节点存储的数据输出为一个单独的网格
		*/
		static Mesh ProcessMesh(struct aiMesh* aMehs, const struct aiScene* aScene);

		/**
		* @brief 使用加载纹理的函数从模型中读取纹理
		*/
		static vector<Texture> LoadMaterialTextures(struct aiMaterial *mat, aiTextureType type, string typeName);


		/**
		* @brief 处理Assimp的节点数据类型，递归形式地处理
		*/
		static void ProcessNode(struct aiNode* aNode, const struct  aiScene* aScene);
	};

#pragma endregion


#pragma region CLASS_IMAGELOADER
	class ImageLoader
	{
	public:
		ImageLoader();
		~ImageLoader();

		static unsigned char* LoadImage(const char* aPath, int* aWidth, int* aheight, int* aChannel,int aReqComp = 0);
		static void FreeData(unsigned char* aData);
	private:

	};


#pragma endregion 

#pragma region CLASS_TEXTLOADER
	class TextLoader
	{
	public:
		TextLoader();
		~TextLoader();

	private:

	};

 
#pragma endregion


