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
   除了加载文件之外，Assimp允许我们设定一些选项来强制它对导入的数据做一些额外的计算或操作。
   通过设定aiProcess_Triangulate，我们告诉Assimp，如果模型不是（全部）由三角形组成，
   它需要将模型所有的图元形状变换为三角形。aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标
   （你可能还记得我们在纹理教程中说过，在OpenGL中大部分的图像的y轴都是反的，所以这个后期处理选项将会修复这个）。

   aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
   aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，
	   只能渲染较小的网格，那么它会非常有用。
   aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
   */
StaticMesh ModelLoader::LoadModel(const string & aPath)
{
	mTempMesh.clear();
	Mesh mtMesh = {};
	textures_loaded.clear();
	directory.clear();
	DebugData("@ModelLoader: Loading...", aPath);
	Assimp::Importer importer;
	
	//翻转纹理y轴并将图元都变成三角形	 

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
	//使用一个循环来遍历StaticMesh中的Mesh数组
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
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
	{
		//这个aiNode节点仅仅存储了Mesh的索引
		//aiScene节点存储了模型的所有数据，而这个aiNode的作用是保证这些数据之间的组织性
		aiMesh *mesh = aScene->mMeshes[aNode->mMeshes[i]];
		/*if (mesh = nullptr)
		{
			DebugLog("Null aiMesh!");
			return {};
		}*/
		mTempMesh.push_back(ProcessMesh(mesh, aScene));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < aNode->mNumChildren; i++)
	{
		 
		//当处理完当前aiNode的所有数据之后，递归处理每一个孩子节点
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

		if (aMehs->mTextureCoords[0]) // 网格是否有纹理坐标？
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

	//处理索引，从aiMesh 中的Face读取索引
	for (unsigned int i = 0; i < aMehs->mNumFaces; i++)
	{
		aiFace face = aMehs->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
			
	}
	
		// 处理材质
	if (aMehs->mMaterialIndex >= 0)
	{
		//从场景节点中读取材质
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
	 /*我们假定在着色器中有一个命名采样器的约定。每个漫反射纹理都应该命名
		如‘texture_diffuseN’，其中N是一个从1到MAX_SAMPLER_NUMBER的连续数字。
		同样适用于其他纹理，如下列表总结 :
		漫反射:texture_diffuseN
		镜面 : texture_specularN
		法线 : texture_normalN
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
