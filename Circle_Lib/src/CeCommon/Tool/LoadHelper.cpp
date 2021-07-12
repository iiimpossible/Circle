#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "LoadHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../OpenGL/CCGLDraw.h"
string  ModelLoader::directory;
vector<Mesh> ModelLoader::mTempMesh;
Mesh ModelLoader::mtMesh;
vector<Texture> ModelLoader::textures_loaded;


void ModelLoader::InitAssimp()
{

}

/*
   ���˼����ļ�֮�⣬Assimp���������趨һЩѡ����ǿ�����Ե����������һЩ����ļ���������
   ͨ���趨aiProcess_Triangulate�����Ǹ���Assimp�����ģ�Ͳ��ǣ�ȫ��������������ɣ�
   ����Ҫ��ģ�����е�ͼԪ��״�任Ϊ�����Ρ�aiProcess_FlipUVs���ڴ����ʱ��תy�����������
   ������ܻ��ǵ�����������̳���˵������OpenGL�д󲿷ֵ�ͼ���y�ᶼ�Ƿ��ģ�����������ڴ���ѡ����޸��������

   aiProcess_GenNormals�����ģ�Ͳ������������Ļ�����Ϊÿ�����㴴�����ߡ�
   aiProcess_SplitLargeMeshes�����Ƚϴ������ָ�ɸ�С����������������Ⱦ����󶥵������ƣ�
	   ֻ����Ⱦ��С��������ô����ǳ����á�
   aiProcess_OptimizeMeshes�����ϸ�ѡ���෴�����Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż���
   */
StaticMesh ModelLoader::LoadModel(const string & aPath)
{
	mTempMesh.clear();
	Mesh mtMesh = {};
	textures_loaded.clear();
	directory.clear();
	DebugData("@ModelLoader: Loading...", aPath);
	Assimp::Importer importer;
	
	//��ת����y�Ტ��ͼԪ�����������	 

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(aPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DebugData("@ModelLoader: ERROR::ASSIMP::", import.GetErrorString()) ;
		return StaticMesh();
	}
	directory = aPath.substr(0, aPath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

	StaticMesh t;
	t.Meshes = mTempMesh;
	t.Init();
	return t;
}

void ModelLoader::TestDraw(const StaticMesh& aTestMesh)
{
	//ʹ��һ��ѭ��������StaticMesh�е�Mesh����
	//for(int i = 0;i<)
	//glBindVertexArray(aTestMesh.VAO);
	//glDrawElements(GL_TRIANGLES,);
	//glBindVertexArray(0);
	//aTestMesh.TestDraw();
	
	
}

void ModelLoader::ProcessNode(aiNode * aNode, const aiScene * aScene)
{
	 
	/*if (aNode = nullptr)
	{
		DebugLog("Null aiNode!");
		return {};
	}*/
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
	{
		//���aiNode�ڵ�����洢��Mesh������
		//aiScene�ڵ�洢��ģ�͵��������ݣ������aiNode�������Ǳ�֤��Щ����֮�����֯��
		aiMesh *mesh = aScene->mMeshes[aNode->mMeshes[i]];
		/*if (mesh = nullptr)
		{
			DebugLog("Null aiMesh!");
			return {};
		}*/
		mTempMesh.push_back(ProcessMesh(mesh, aScene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < aNode->mNumChildren; i++)
	{
		 
		//�������굱ǰaiNode����������֮�󣬵ݹ鴦��ÿһ�����ӽڵ�
		ProcessNode(aNode->mChildren[i], aScene);
	}


}

Mesh ModelLoader::ProcessMesh(aiMesh * aMehs, const aiScene * aScene)
{
	vector<Vertex> vertices;
	vector<unsigned> indices;
	vector<Texture> textures;

	for (unsigned i = 0; i < aMehs->mNumVertices; i++)
	{
		Vertex vertex;
		Vec3 vector;
		vector.x = aMehs->mVertices[i].x;
		vector.y = aMehs->mVertices[i].y;
		vector.z = aMehs->mVertices[i].z;
		vertex.Position = vector;
		

		if (aMehs->HasNormals())
		{
			vector.x = aMehs->mNormals[i].x;
			vector.y = aMehs->mNormals[i].y;
			vector.z = aMehs->mNormals[i].z;
			vertex.Normal = vector;
		}

		vector.x = aMehs->mNormals[i].x;
		vector.y = aMehs->mNormals[i].y;
		vector.z = aMehs->mNormals[i].z;
		vertex.Normal = vector;

		if (aMehs->mTextureCoords[0]) // �����Ƿ����������ꣿ
		{
			Vec2 vec;
			vec.x = aMehs->mTextureCoords[0][i].x;
			vec.y = aMehs->mTextureCoords[0][i].y;
			vertex.TexCoord = vec;
		}
		else
		{
			vertex.TexCoord = Vec2(0.0f, 0.0f);
		}
			
		vertices.push_back(vertex);
	}

	//������������aiMesh �е�Face��ȡ����
	for (unsigned int i = 0; i < aMehs->mNumFaces; i++)
	{
		aiFace face = aMehs->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
			
	}
	
		// �������
	if (aMehs->mMaterialIndex >= 0)
	{
		//�ӳ����ڵ��ж�ȡ����
		aiMaterial *material = aScene->mMaterials[aMehs->mMaterialIndex];
		vector<Texture> diffuseMaps = LoadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = LoadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}



	// process materials
	aiMaterial* material = aScene->mMaterials[aMehs->mMaterialIndex];
	 /*���Ǽٶ�����ɫ������һ��������������Լ����ÿ������������Ӧ������
		�确texture_diffuseN��������N��һ����1��MAX_SAMPLER_NUMBER���������֡�
		ͬ���������������������б��ܽ� :
		������:texture_diffuseN
		���� : texture_specularN
		���� : texture_normalN
		*/
	 

	// 1. diffuse maps
	vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


	
	return Mesh { vertices,indices ,textures };
	//return Mesh(vertices, indices, textures);
	 
}

vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
	return vector<Texture>();
}

unsigned int ModelLoader::TextureFromFile(const char * path, const string & directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;	
	glGenTextures(1, &textureID);
	int width, height, nrComponents;	
	unsigned char *data = ImageLoader::LoadImage(filename.c_str(), &width, &height, &nrComponents);

	if (data)
	{
		textureID = CircleEngine::CreateTexture2D(data, width, height, nrComponents).TextureHandle;		 
		ImageLoader::FreeData(data);
	}
	else
	{
		DebugData("@TextureFromFile: Texture failed to load at path: ", path);
		ImageLoader::FreeData(data);
		return -1;
	}
	return textureID;
}



ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}













ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}

unsigned char * ImageLoader::LoadImage(const char * aPath, int* aWidth, int* aheight, int* aChannel, int aReqComp)
{
	unsigned char *data =  stbi_load(aPath, aWidth, aheight, aChannel, aReqComp);
	if (data == nullptr)
	{
		DebugLog("@ImageLoader::LoadImage: Load Failed!");
		return nullptr;
	}
	return data;
}

void ImageLoader::FreeData(unsigned char * aData)
{
	stbi_image_free(aData);
}

TextLoader::TextLoader()
{
}

TextLoader::~TextLoader()
{
}
