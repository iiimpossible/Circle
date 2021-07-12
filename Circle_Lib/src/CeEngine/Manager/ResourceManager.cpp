#include "ResourceManager.h"
#include <CeCommon/Common.h>
#include <CeCommon/Tool/MaterialLexer.h>
#include <CeCommon/Tool/CeLoader.h>
#include <CeCommon/Tool/CeUtility.h>
#include <CeCommon/OpenGL/CeShaderCompiler.h>
#include <CeCommon/OpenGL/CeTextureFomatter.h>
#include <CeCommon/OpenGL/CeMeshFormatter.h>
#include <CeCommon/OpenGL/CeTextureFomatter.h>
#include <CeEngine/Manager/DebugLogManager.h>
#include <CeEngine/Render/Texture2D.h>
#include <CeEngine/Render/Shader.h>
#include <CeEngine/Render/StaticMesh.h>
#include <CeEngine/Render/Material.h>

using namespace CircleEngine;
using namespace std;


std::map<std::string, Texture2D> ResourceManager::mTextures;
std::map<std::string, Shader> ResourceManager::mShaders;
std::unordered_map<string, Material> ResourceManager::mMaterials;
std::map<string, StaticMesh> ResourceManager::mStaticMeshes;
std::unordered_map<string, unsigned> ResourceManager::mTextureCubes;
//std::map<unsigned char , Character> ResourceManager::characters;
#include <CeEngine/Data/InlineData.h>
ResourceManager::ResourceManager() 
{
	mTextures.insert(pair<string, Texture2D>("zero", {}));
	mShaders.insert(pair<string, Shader>("zero", {}));
	mMaterials.insert(pair<string, Material>("zero", {}));
	mStaticMeshes.insert(pair<string, StaticMesh>("zero", {}));
}
ResourceManager::~ResourceManager() {}

void ResourceManager::Start()
{
	//Ĭ����Ϸ��������
	StaticMesh pre_cube;	 
	pre_cube.SetVAO(CeMeshFormatter::CreateVAO());
	pre_cube.SetVBO(CeMeshFormatter::CreateVBO(pre_cube.GetMultiVAO()[0], make_tuple(&InlineMeshData::Cube_Vertices[0], InlineMeshData::Cube_Vertices.size(), 3, 3),
		make_tuple(&InlineMeshData::Cube_TexCoords[0], InlineMeshData::Cube_TexCoords.size(), 2, 2),
		make_tuple(&InlineMeshData::Cube_Normals[0], InlineMeshData::Cube_Normals.size(), 3, 3)));
	pre_cube.SetEBO(CeMeshFormatter::CreateEBO(pre_cube.GetMultiVAO()[0], make_tuple(&InlineMeshData::Cube_Indices[0], InlineMeshData::Cube_Indices.size())));
	pre_cube.SetIndicesNum(InlineMeshData::Cube_Indices.size());
	mStaticMeshes.insert(pair<string, StaticMesh>("Pre_Cube", pre_cube));

	//Ĭ�ϲ���
	Material pre_mat;
	unsigned hd = CeShaderCompiler::CompileShader(InlineShaderCode::PureColor_VS3D, InlineShaderCode::PureColor_FS3D);	
	pre_mat.AddShaderElement(hd, {}, {});
	mMaterials.insert(pair<string, Material>("Pre_StandardMat", pre_mat));


	//unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/starfield/starfield.tga");
	//unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/sunset/sunset.tga");
	//unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/standard/standard.jpg");
	unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/Pre_Skybox.jpg");
	mTextureCubes.insert(pair<string, unsigned>("Pre_Skybox", pre_cubemap));

	//��պ����������ɫ��
	Shader skybox_shader;
	skybox_shader.mShaderID = CeShaderCompiler::CompileShader(CeLoader::LoadText("asset/material/shader/skybox_v.shader"), 
		CeLoader::LoadText("asset/material/shader/skybox_f.shader"));
	skybox_shader.mVAO = pre_cube.GetMultiVAO()[0];//��������Ƚ�Σ��
	skybox_shader.mVBO = pre_cube.GetMultiVBO()[0];
	skybox_shader.mVerticesNum = pre_cube.GetIndicesNum()[0];
	skybox_shader.mTextureID = pre_cubemap;	
	mShaders.insert(pair<string, Shader>("Pre_Skybox", skybox_shader));	

	//��Ӱӳ����ɫ��
	//Shader shadowmap_shader;
	//shadowmap_shader.mShaderID = CeShaderCompiler::CompileShader(CeLoader::LoadText("asset/material/shader/shadowmap_v.shader"),
	//	CeLoader::LoadText("asset/material/shader/shadowmap_f.shader"));
	//mShaders.insert(pair<string, Shader>("Pre_ShadowMap", shadowmap_shader));
}

/**
 * @brief ���ط�������
 * @detail ������ȡ�ı�------>�ʷ������õ��ؼ���Ϣ������·������ɫ����----->��������
 * ---->������ɫ��----->�洢�������ɫ�����
 */
int ResourceManager::LoadMaterialOld(const string& aPath)
{
	string file_name = CeUtility::GetFileNameFromPath(aPath);
	//�ظ����
	auto iter = mMaterials.find(file_name);
	if (iter != mMaterials.end())
	{
		//ReloadMaterial(file_name, aPath);
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name repeated, material reload." });
		return 0;//δ��֧�ֲ��ʽű����±��룬ȥ����������ʽű����±�����Ҫ��¼���øò��ʵ���Ϸ���壬���ڽű�����
		//����֮��������ò��ʵ���Ϸ����Ĳ���
	}
	//�������ʽű��������ʱ����
	vector<string> tex_names;
	vector<string> tex_paths;
	ShaderCode code;
	string mat_script;
	//�����ʱ����
	Material mat;
	vector<unsigned> tex_hds;
	int state = 0;//���״̬��ǣ�����ĸ��׶��д�������state<0����ִ���ͷź���


	//�����ݴ���mapǰ��������ʱ�����Ա�����󷵻�
	
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name ", CeUtility::GetFileNameFromPath(aPath) });
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material path ", aPath });
	//�Ӳ����ļ��ж�ȡ���ʽű�
	mat_script =  CeLoader::LoadText(aPath);
	if (mat_script.empty())
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Material file path exception. " });
		return 0;
	}
	CeMaterialLexer::LexicalAnalysis(mat_script,&tex_names,&tex_paths,&code);//�������ʽű�
	//DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { CeMaterialLexer::GetDebugLog()});//����ǲ��ʽű�����log
	
	//DebugLogManager::DebugOutput("ResourceManager", EDebugTag::EDT_Log, tex_names);
	//DebugLogManager::DebugOutput("ResourceManager", EDebugTag::EDT_Log, tex_paths);
	//��������
	int i = 0;
	while (i<tex_names.size() && i< tex_paths.size())
	{
		auto tex = LoadTexture2D(tex_paths[i]);//����û�н����ɵ�����2D�浽map��
		if (!tex)
		{			
			DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Texture file path exception. "});
			//�ͷ�֮ǰ���ص�����
			CeTextureFormatter::DeleteTexture2D(tex_hds);			
			return -2;
		}
		tex_hds.push_back(tex);
		i++;
	}

	//������ɫ��	
	unsigned shader_hd = 
		CeShaderCompiler::CompileShader(code.vertex.c_str(),code.fragment.c_str());
	if (!shader_hd)
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Shader compile exception." });
		//�ͷ�֮ǰ���ص�����
		CeTextureFormatter::DeleteTexture2D(tex_hds);
		return -3;
	}

	//�󶨲��ʵ���ɫ��
	CeShaderCompiler::SetTextureUnitName(shader_hd, tex_names);
	for (int i = 0; i < tex_hds.size(); i++)
	{
		
		CeTextureFormatter::BindTexture2D(shader_hd,tex_hds[i],i);
	}

	//�����ʴ洢��map��
	mat.AddTexture(tex_hds);
	mat.AddShader(shader_hd);  //�����಻֧�ֵ�����ɫ����

	mMaterials.insert(pair<string, Material>(file_name, mat));//TODO: �������Ϊ��·��������ȡ���ʵ�����
	return 1;
}


/*==============================================================================================================*/


int ResourceManager::LoadMaterial(const string & aPath, const string& aName)
{
	
	string file_name;//��·������ļ���
	if (aName.empty())
	{
		file_name = CeUtility::GetFileNameFromPath(aPath);
	}
	else
	{
		file_name = aName;
	}

	//�ظ����
	auto iter = mMaterials.find(file_name);
	if (iter != mMaterials.end())
	{
		//ReloadMaterial(file_name, aPath);
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name repeated, material reload." });
		return 0;//δ��֧�ֲ��ʽű����±��룬ȥ����������ʽű����±�����Ҫ��¼���øò��ʵ���Ϸ���壬���ڽű�����
		//����֮��������ò��ʵ���Ϸ����Ĳ���
	}

	//�������ʽű��������ʱ����
	vector<ShaderPrototype> shader_pros;
	MaterialPrototype material_pros;
	string mat_script;
	//�����ʱ����
	Material mat; mat.Clear();
	//�����ݴ���mapǰ��������ʱ�����Ա�����󷵻�

	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name ", CeUtility::GetFileNameFromPath(aPath) });
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material path ", aPath });
	//�Ӳ����ļ��ж�ȡ���ʽű�
	mat_script = CeLoader::LoadText(aPath);
	if (mat_script.empty())
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Material file path exception. " });
		return 0;
	}
	int lex_tag =  CeMaterialLexer::LexicalAnalysis(mat_script, &shader_pros,&material_pros);//�������ʽű�
	if (!lex_tag)
	{
		DebugLogManager::DebugOutput("Resourcemanager", EDebugTag::EDT_Log, { CeMaterialLexer::GetDebugLog() });
		return 0;
	}
	
	for (auto val : shader_pros)
	{
		//��������
		vector<unsigned> hds;//һ����ɫ��������
		for (int i = 0; i < val.texture_name.size() && i < val.texture_path.size(); i++)
		{
			unsigned hd = LoadTexture2D(val.texture_path[i]);

			if (!hd)
			{
				DebugLogManager::DebugOutput("ResourceManger", EDebugTag::EDT_ERROR, { "Texture load error." });
				CeTextureFormatter::DeleteTexture2D(mat.GetTextureHds());
				CeShaderCompiler::DeleteShaderProgram(mat.GetShaderHds());
				return 0;
			}
			mat.AddTexture(hd);
			hds.push_back(hd);
		}

		//������ɫ��
		unsigned shader =
			CeShaderCompiler::CompileShader(val.vertex, val.fragment, val.geometry);
		
		if (!shader)
		{			
			//DebugLogManager::DebugOutput("ResourceManger", EDebugTag::EDT_ERROR, {"Vertex code is: \n" ,val.vertex, "\n", "Fragment code is: \n"  ,val.fragment });
			CeTextureFormatter::DeleteTexture2D(mat.GetTextureHds());
			CeShaderCompiler::DeleteShaderProgram(mat.GetShaderHds());
			return 0;
		}		
		//������	 
		

		mat.AddShader(shader);
		mat.AddShaderElement(shader, hds, val.texture_name);
		mat.SetEmissiveColor(material_pros.color[0], material_pros.color[1], material_pros.color[2]);	
		mat.SetPath(aPath);		
	}

	DebugLogManager::DebugOutput("ResourceManger", EDebugTag::EDT_Log, { "Material load success." });
	mMaterials.insert(pair<string, Material>(file_name, mat));//����map��
	
	return 1;
}

/*==============================================================================================================*/

int ResourceManager::ReloadMaterial(const string& aName, const string& aPath)
{
	//��ɾ��֮ǰ�ģ��ᵼ��֮ǰ����Ϸ������ȾʧЧ
	CeTextureFormatter::DeleteTexture2D(mMaterials[aName].GetTextureHds());
	CeShaderCompiler::DeleteShaderProgram(mMaterials[aName].GetShaderHds());


	//�������ʽű��������ʱ����
	vector<string> tex_names;
	vector<string> tex_paths;
	ShaderCode code;
	string mat_script;
	//�����ʱ����
	Material mat;
	vector<unsigned> tex_hds;
	int state = 0;//���״̬��ǣ�����ĸ��׶��д�������state<0����ִ���ͷź���


	//�����ݴ���mapǰ��������ʱ�����Ա�����󷵻�

	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name ", CeUtility::GetFileNameFromPath(aPath) });
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material path ", aPath });
	//�Ӳ����ļ��ж�ȡ���ʽű�
	mat_script = CeLoader::LoadText(aPath);
	if (mat_script.empty())
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Material file path exception. " });
		return 0;
	}
	CeMaterialLexer::LexicalAnalysis(mat_script, &tex_names, &tex_paths, &code);//�������ʽű�
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { CeMaterialLexer::GetDebugLog() });//����ǲ��ʽű�����log

	//��������
	int i = 0;
	while (i < tex_names.size() && i < tex_paths.size())
	{
		tex_names[i];
		tex_paths[i];
		auto tex = LoadTexture2D(tex_paths[i]);//����û�н����ɵ�����2D�浽map��
		if (!tex)
		{
			DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Texture file path exception. " });
			//�ͷ�֮ǰ���ص�����
			CeTextureFormatter::DeleteTexture2D(tex_hds);
			return -2;
		}
		tex_hds.push_back(tex);
		i++;
	}

	//������ɫ��	
	unsigned shader_hd =
		CeShaderCompiler::CompileShader(code.vertex.c_str(), code.fragment.c_str());
	if (!shader_hd)
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Shader compile exception." });
		//�ͷ�֮ǰ���ص�����
		CeTextureFormatter::DeleteTexture2D(tex_hds);
		return -3;
	}

	//�󶨲��ʵ���ɫ��
	CeShaderCompiler::SetTextureUnitName(shader_hd, tex_names);
	for (int i = 0; i < tex_hds.size(); i++)
	{
		CeTextureFormatter::BindTexture2D(shader_hd, tex_hds[i], i);
	}

	//�����ʴ洢��map��
	mat.AddTexture(tex_hds);
	mat.AddShader(shader_hd); 

	mMaterials[aName] = mat;
	return 1;
}


/*==============================================================================================================*/


unsigned ResourceManager::LoadTexture2D(const string & aPath)
{
	int width, height, channel;
	unsigned tex_hd;
	unsigned char* data = CeLoader::LoadImageData(aPath.c_str(),&width,&height,&channel);

	//DebugLogManager::DebugOutput("Resourcemanager",EDebugTag::EDT_Log, { "Channel is: ",TO_STR(channel) });
	if (!data)
	{
		DebugLogManager::DebugOutput("Resourcemanager",EDebugTag::EDT_ERROR, { "Image load exception, perhaps path invalid. Path is ",  aPath });
		return 0;
	}
		
	tex_hd = CeTextureFormatter::CreateTexture2D(data,width,height,channel).texHD;
	if (!tex_hd)
	{
		DebugLogManager::DebugOutput("Resourcemanager",EDebugTag::EDT_ERROR, { "Texture2D create exception, perhaps graphic function error. Path is ",  aPath , "Handle is ", TO_STR(tex_hd)});
		return 0;
	}
	CeLoader::FreeImageData(data);
	return tex_hd;
}

unsigned ResourceManager::LoadTextureCube(const string & aPath)
{
	if (aPath.empty()) return 0;

	int width, height, channel;
	unsigned tex_hd;
	vector<unsigned char*> datas = CeLoader::LoadCubeImageData(aPath, &width, &height, &channel);
	tex_hd = CeTextureFormatter::CreateTextureCube(width,height,channel,datas);
	CeLoader::FreeCubeImageData(datas);
	if (tex_hd)
	{
		mTextureCubes.insert(pair < string, unsigned>(CeUtility::GetFileNameFromPath(aPath), tex_hd));
	}	
	return tex_hd;
}


/*==============================================================================================================*/

/**
 * @brief ����ģ�Ͳ��洢
 * @TODO �Զ���ȡ������ʺ����֣����ɲ���ģ�壨����·���Ѿ�д�ã�
 */
int ResourceManager::LoadStaticMesh(const string& aPath)
{
	string file_name = CeUtility::GetFileNameFromPath(aPath);
	//�ظ����
	auto iter = mStaticMeshes.find(file_name);
	if (iter != mStaticMeshes.end())
	{
		DebugLogManager::DebugOutput({ "@ResourceManager ERROR: Model name repeated." });
		return 0;
	}

	StaticMesh st_mesh;
	vector<unsigned> vec_vaos;
	vector<unsigned> vec_vbos;
	vector<unsigned> vec_ebos;
	vector<unsigned> indices_num;

	auto t_mesh = CeLoader::LoadModel(aPath);

	if (t_mesh.empty())
	{
		DebugLogManager::DebugOutput({ "@ResourceManager ERROR: Load model exception." });
		return 0;
	}
	for (auto val : t_mesh)
	{
		unsigned t_vao = CeMeshFormatter::CreateVAO();
		unsigned t_vbo = CeMeshFormatter::CreateVBO(t_vao, make_tuple(&val.vertices[0], val.vertices.size(), 3, 3),
			make_tuple(&val.texcoords[0], val.texcoords.size(), 2, 2),
			make_tuple(&val.normals[0], val.normals.size(), 3, 3));
		unsigned t_ebo = CeMeshFormatter::CreateEBO(t_vao, make_tuple(&val.indices[0], val.indices.size()));
		vec_vaos.push_back(t_vao);
		vec_vbos.push_back(t_vbo);
		vec_ebos.push_back(t_ebo);
		indices_num.push_back(val.indices.size());		
	}
	
	st_mesh.SetMultiVAO(vec_vaos);
	st_mesh.SetMultiVBO(vec_vbos);
	st_mesh.SetMultiEBO(vec_ebos);
	st_mesh.SetMultiIndicesNum(indices_num);
	DebugLogManager::DebugOutput({ "@ResourceManager LOG: Mesh num is ", to_string(st_mesh.GetMultiVAO().size()) });
	DebugLogManager::DebugOutput({ "@ResourceManager LOG: vbo num is ", to_string(st_mesh.GetMultiVBO().size()) });
	DebugLogManager::DebugOutput({ "@ResourceManager LOG: ebo num is ", to_string(st_mesh.GetMultiEBO().size()) });
	DebugLogManager::DebugOutput({ "@ResourceManager LOG: Model name is ",CeUtility::GetFileNameFromPath(aPath) });
	mStaticMeshes.insert(pair<string, StaticMesh>(CeUtility::GetFileNameFromPath(aPath), st_mesh));
	return 1;
}

 
/*==============================================================================================================*/


StaticMesh & ResourceManager::GetStaticMesh(const string & aName)
{
	auto iter = mStaticMeshes.find(aName);
	if (iter == mStaticMeshes.end())
	{
		DebugLogManager::DebugOutput({ "@ResourceManager ERROR: Model not exist." });
		return mStaticMeshes["zero"];
	}
	return mStaticMeshes[aName];	
}

vector<string> ResourceManager::GetMaterialList()
{
	vector<string> mats;
	auto iter = mMaterials.begin();

	for (; iter != mMaterials.end(); iter++)
	{
		mats.push_back((*iter).first);
	}
	return mats;
}

bool ResourceManager::FindMaterial(const string & aName)
{
	if (mMaterials.find(aName) != mMaterials.end())
		return true;
	else
		return false;
}


/*==============================================================================================================*/

Material& ResourceManager::GetMaterial(const string & aName)
{
	auto iter = mMaterials.find(aName);	
	if (iter == mMaterials.end())
	{
		DebugLogManager::DebugOutput({ "@ResourceManager ERROR: Material not exist." });
		return mMaterials["zero"];
	}	
	return (*iter).second;	
}

Texture2D ResourceManager::LoadTexture(const char* file, string name)
{
	mTextures[name] = LoadTextureFromFile(file);
	return ResourceManager::mTextures[name];
}

Texture2D & ResourceManager::GetTexture(string name)
{	 
	auto iter = mTextures.find(name);
	if (iter == mTextures.end())
	{
		DebugLogManager::DebugOutput({ "@ResourceManager::GetShader: Texture Not Exist!" });
		return mTextures["zero"];
	}
	return ResourceManager::mTextures[name];
}

Texture2D ResourceManager::LoadTextureFromFile(const char* aPath)
{	
	int width = 0, height = 0, channel = 0;	
	unsigned char* data = CeLoader::LoadImageData(aPath, &width, &height, &channel);
	if (!data)
	{
		DebugLogManager::DebugOutput({"@ResourceManager ERROR: Texture path not exist."});
		return {};
	}
	//����һ��2D������������������Ϣ����ʹ��Ĭ�ϻ��ƺ͹��ˣ������ƺ͹��˿�����Texture�����޸�
	Texture2DInfo info = CeTextureFormatter::CreateTexture2D(data, width, height, channel);
	Texture2D texture(info);
	CeLoader::FreeImageData(data);
	DebugLogManager::DebugOutput("ResourceManger", EDebugTag::EDT_Log, { "Texture path is: ",aPath });
	return texture;
}


Shader& ResourceManager::GetShader(string aName)
{
	auto iter = mShaders.find(aName);
	if (iter == mShaders.end())
	{
		DebugLogManager::DebugOutput("ResourceManager",EDebugTag::EDT_ERROR,{ "Shader not exist! Input name is: " , aName });
		return mShaders["zero"];
	}
	return ResourceManager::mShaders[aName];
}





 
 