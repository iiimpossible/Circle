
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
		* @brief ����ģ�ͺ���
		* ͨ������·������Assimp��ĺ���������ģ�ͣ���ת��ΪOpenGL����ʶ�������
		* @param aPath ģ�͵�·��
		* @return StaticMesh ���Ϊһ����̬�����񡣸þ�̬������ܰ����˶��������
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
		* @brief ��Assimp�ڵ�洢���������Ϊһ������������
		*/
		static Mesh ProcessMesh(struct aiMesh* aMehs, const struct aiScene* aScene);

		/**
		* @brief ʹ�ü�������ĺ�����ģ���ж�ȡ����
		*/
		static vector<Texture> LoadMaterialTextures(struct aiMaterial *mat, aiTextureType type, string typeName);


		/**
		* @brief ����Assimp�Ľڵ��������ͣ��ݹ���ʽ�ش���
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


