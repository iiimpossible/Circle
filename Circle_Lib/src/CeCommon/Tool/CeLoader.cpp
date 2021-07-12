#include "CeLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <Celib_x64/lib/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <CeEngine/Manager/DebugLogManager.h>
#include <CeCommon/Tool/CeUtility.h>
#include "MaterialLexer.h"

using std::ifstream;
vector<CeMesh> CeLoader::mTempMesh;

string CeLoader::LoadText(const string& aPath)
{
	std::string line;
	string result;
	ifstream fstream(aPath);
	if (!fstream.is_open())
	{		
		DebugLogManager::DebugOutput("CeLoader", EDebugTag::EDT_ERROR, { "Path not valid.","[",aPath,"]"});
		return result;
	}	
	while (getline(fstream, line))
	{
		result += line;
		result.push_back('\n');
	}
	fstream.close();
	if (result.empty())
	{
		DebugLogManager::DebugOutput("CeLoader", EDebugTag::EDT_ERROR, {"File empty.","[",aPath,"]" });
		return result;
	}
	return result;
}

string CeLoader::GetFileNameFromPath(const string & aPath)
{	
	string str;
	for (auto val : aPath)
	{
		str.push_back(val);
		if ((val == '\\') || (val == '/'))
		{
			str.clear();
		}
	}
	return str;
}


void CeLoader::LoadMaterial(const char* aPath, vector<string>* aTexName, vector<string>* aTexPath, vector<string>* aShaderCode)
{
	const char* aCharStr = LoadText(aPath).c_str();
	ShaderCode sc;
	CeMaterialLexer::LexicalAnalysis(aCharStr, aTexName, aTexPath, &sc);
	(*aShaderCode).push_back(sc.vertex);
	(*aShaderCode).push_back(sc.fragment);
	(*aShaderCode).push_back(sc.geometry);
}


unsigned char * CeLoader::LoadImageData(const char * aPath, int* aWidth, int* aheight, int* aChannel, int aReqComp)
{
	unsigned char *data = stbi_load(aPath, aWidth, aheight, aChannel, aReqComp);
	if (data == nullptr)
	{
		DebugLogManager::DebugOutput("CeLoader", EDebugTag::EDT_ERROR, { "Image load failed, image nullptr." });		
		return nullptr;
	}
	return data;
}

vector<unsigned char*> CeLoader::LoadCubeImageData(const string & aPath, int * aWidth, int * aHeight, int * aChannel, int aReqComp)
{
	vector<std::string> faces
	{
		"right",
		"left",
		"top",
		"bottom",
		"front",
		"back"
	};
	string path;
	string front;
	string suffix;
	CeUtility::SplitStringFromSuffix(aPath,front,suffix);
	DebugLogManager::DebugOutput("Celoader", EDebugTag::EDT_Log, { front, " ", suffix });
	unsigned char *data;
	vector<unsigned char*> res;

	for (int i = 0; i < 6; i++)
	{
		path = front + '_' + faces[i] + suffix;
		data = stbi_load(path.c_str(), aWidth, aHeight, aChannel, aReqComp);
		if (data == nullptr)
		{
			DebugLogManager::DebugOutput("CeLoader", EDebugTag::EDT_ERROR, { "Image load failed, image nullptr. Path is: ",path });
			return {};
		}
		res.push_back(data);
	}	
	return res;
}

void CeLoader::FreeImageData(unsigned char * aData)
{
	stbi_image_free(aData);
}

void CeLoader::FreeCubeImageData(vector<unsigned char*> aData)
{
	for (auto data : aData)
	{
		if (data == nullptr) continue;
		FreeImageData(data);
	}
}


vector<CeMesh>& CeLoader::LoadModel(const string& aPath)
{
	mTempMesh.clear();
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(aPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DebugLogManager::DebugOutput("CeLoader", EDebugTag::EDT_ERROR, { "Assimp error: " ,import.GetErrorString() });		
		return mTempMesh;
	}
	ProcessNode(scene->mRootNode, scene);
	return mTempMesh;
}


void CeLoader::ProcessNode(aiNode * aNode, const aiScene * aScene)
{
	//处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
	{
		//这个aiNode节点仅仅存储了Mesh的索引
		//aiScene节点存储了模型的所有数据，而这个aiNode的作用是保证这些数据之间的组织性
		aiMesh *mesh = aScene->mMeshes[aNode->mMeshes[i]];
		mTempMesh.push_back(ProcessMesh(mesh, aScene));
	}
	//接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < aNode->mNumChildren; i++)
	{
		//当处理完当前aiNode的所有数据之后，递归处理每一个孩子节点
		ProcessNode(aNode->mChildren[i], aScene);
	}
}


CeMesh CeLoader::ProcessMesh(aiMesh * aMehs, const aiScene * aScene)
{
	vector<float> vertices;
	vector<float> normals;
	vector<float> tex_coords;
	vector<unsigned> indices;

	for (unsigned i = 0; i < aMehs->mNumVertices; i++)
	{
		vertices.push_back(aMehs->mVertices[i].x);
		vertices.push_back(aMehs->mVertices[i].y);
		vertices.push_back(aMehs->mVertices[i].z);
		if (aMehs->HasNormals())
		{
			normals.push_back(aMehs->mNormals[i].x);
			normals.push_back(aMehs->mNormals[i].y);
			normals.push_back(aMehs->mNormals[i].z);
		}
		if (aMehs->mTextureCoords[0]) // 网格是否有纹理坐标？
		{
			tex_coords.push_back(aMehs->mTextureCoords[0][i].x);
			tex_coords.push_back(aMehs->mTextureCoords[0][i].y);
		}
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
	CeMesh mesh;
	mesh.vertices = vertices;
	mesh.normals = normals;
	mesh.texcoords = tex_coords;
	mesh.indices = indices;
	return mesh;
}

//void LoadText(const char * aPath, char** aText)
//{
//	string str = LoadManager::LoadText(aPath);
//	if (str.empty()) return;
//	(*aText) = new char[str.size()+1];
//	str.copy((*aText), str.size(), 0);
//	(*aText)[str.size()] = '\0';
//}
//
//unsigned char* LoadImageData(const char * aPath, int * aWidth, int * aHeight, int * aChannel)
//{
//	unsigned char* data = LoadManager::LoadImageData(aPath,aWidth,aHeight,aChannel);
//	return data;
//}
//
//void FreeImage(unsigned char * aData)
//{
//	LoadManager::FreeImageData(aData);
//}
//
//void LoadModel(const char * aPath, int* aMeshNum, CeMesh aMesh[])
//{
//	auto mesh = LoadManager::LoadModel(aPath);
//	(*aMeshNum) = mesh.size();
//	
//	//aMesh = new sizeof(CeLoadMesh)
//}
