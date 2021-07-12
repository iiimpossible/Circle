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
	//默认游戏物体网格
	StaticMesh pre_cube;	 
	pre_cube.SetVAO(CeMeshFormatter::CreateVAO());
	pre_cube.SetVBO(CeMeshFormatter::CreateVBO(pre_cube.GetMultiVAO()[0], make_tuple(&InlineMeshData::Cube_Vertices[0], InlineMeshData::Cube_Vertices.size(), 3, 3),
		make_tuple(&InlineMeshData::Cube_TexCoords[0], InlineMeshData::Cube_TexCoords.size(), 2, 2),
		make_tuple(&InlineMeshData::Cube_Normals[0], InlineMeshData::Cube_Normals.size(), 3, 3)));
	pre_cube.SetEBO(CeMeshFormatter::CreateEBO(pre_cube.GetMultiVAO()[0], make_tuple(&InlineMeshData::Cube_Indices[0], InlineMeshData::Cube_Indices.size())));
	pre_cube.SetIndicesNum(InlineMeshData::Cube_Indices.size());
	mStaticMeshes.insert(pair<string, StaticMesh>("Pre_Cube", pre_cube));

	//默认材质
	Material pre_mat;
	unsigned hd = CeShaderCompiler::CompileShader(InlineShaderCode::PureColor_VS3D, InlineShaderCode::PureColor_FS3D);	
	pre_mat.AddShaderElement(hd, {}, {});
	mMaterials.insert(pair<string, Material>("Pre_StandardMat", pre_mat));


	//unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/starfield/starfield.tga");
	//unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/sunset/sunset.tga");
	//unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/standard/standard.jpg");
	unsigned pre_cubemap = LoadTextureCube("asset/texture/skybox/Pre_Skybox.jpg");
	mTextureCubes.insert(pair<string, unsigned>("Pre_Skybox", pre_cubemap));

	//天空盒子网格和着色器
	Shader skybox_shader;
	skybox_shader.mShaderID = CeShaderCompiler::CompileShader(CeLoader::LoadText("asset/material/shader/skybox_v.shader"), 
		CeLoader::LoadText("asset/material/shader/skybox_f.shader"));
	skybox_shader.mVAO = pre_cube.GetMultiVAO()[0];//这个操作比较危险
	skybox_shader.mVBO = pre_cube.GetMultiVBO()[0];
	skybox_shader.mVerticesNum = pre_cube.GetIndicesNum()[0];
	skybox_shader.mTextureID = pre_cubemap;	
	mShaders.insert(pair<string, Shader>("Pre_Skybox", skybox_shader));	

	//阴影映射着色器
	//Shader shadowmap_shader;
	//shadowmap_shader.mShaderID = CeShaderCompiler::CompileShader(CeLoader::LoadText("asset/material/shader/shadowmap_v.shader"),
	//	CeLoader::LoadText("asset/material/shader/shadowmap_f.shader"));
	//mShaders.insert(pair<string, Shader>("Pre_ShadowMap", shadowmap_shader));
}

/**
 * @brief 加载分析材质
 * @detail 包括读取文本------>词法解析得到关键信息（纹理路径和着色器）----->加载纹理
 * ---->编译着色器----->存储纹理和着色器句柄
 */
int ResourceManager::LoadMaterialOld(const string& aPath)
{
	string file_name = CeUtility::GetFileNameFromPath(aPath);
	//重复检测
	auto iter = mMaterials.find(file_name);
	if (iter != mMaterials.end())
	{
		//ReloadMaterial(file_name, aPath);
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name repeated, material reload." });
		return 0;//未来支持材质脚本重新编译，去掉这个。材质脚本重新编译需要记录引用该材质的游戏物体，并在脚本重新
		//编译之后更新引用材质的游戏物体的材质
	}
	//解析材质脚本所需的临时变量
	vector<string> tex_names;
	vector<string> tex_paths;
	ShaderCode code;
	string mat_script;
	//句柄临时变量
	Material mat;
	vector<unsigned> tex_hds;
	int state = 0;//这个状态标记，如果哪个阶段有错误（设置state<0）就执行释放函数


	//将数据存入map前创建的临时变量以避免错误返回
	
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name ", CeUtility::GetFileNameFromPath(aPath) });
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material path ", aPath });
	//从材质文件中读取材质脚本
	mat_script =  CeLoader::LoadText(aPath);
	if (mat_script.empty())
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Material file path exception. " });
		return 0;
	}
	CeMaterialLexer::LexicalAnalysis(mat_script,&tex_names,&tex_paths,&code);//解析材质脚本
	//DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { CeMaterialLexer::GetDebugLog()});//这个是材质脚本解析log
	
	//DebugLogManager::DebugOutput("ResourceManager", EDebugTag::EDT_Log, tex_names);
	//DebugLogManager::DebugOutput("ResourceManager", EDebugTag::EDT_Log, tex_paths);
	//加载纹理
	int i = 0;
	while (i<tex_names.size() && i< tex_paths.size())
	{
		auto tex = LoadTexture2D(tex_paths[i]);//这里没有将生成的纹理2D存到map中
		if (!tex)
		{			
			DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Texture file path exception. "});
			//释放之前加载的纹理
			CeTextureFormatter::DeleteTexture2D(tex_hds);			
			return -2;
		}
		tex_hds.push_back(tex);
		i++;
	}

	//编译着色器	
	unsigned shader_hd = 
		CeShaderCompiler::CompileShader(code.vertex.c_str(),code.fragment.c_str());
	if (!shader_hd)
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Shader compile exception." });
		//释放之前加载的纹理
		CeTextureFormatter::DeleteTexture2D(tex_hds);
		return -3;
	}

	//绑定材质到着色器
	CeShaderCompiler::SetTextureUnitName(shader_hd, tex_names);
	for (int i = 0; i < tex_hds.size(); i++)
	{
		
		CeTextureFormatter::BindTexture2D(shader_hd,tex_hds[i],i);
	}

	//将材质存储到map中
	mat.AddTexture(tex_hds);
	mat.AddShader(shader_hd);  //材质类不支持单个着色器了

	mMaterials.insert(pair<string, Material>(file_name, mat));//TODO: 将这个改为从路径的最后读取材质的名字
	return 1;
}


/*==============================================================================================================*/


int ResourceManager::LoadMaterial(const string & aPath, const string& aName)
{
	
	string file_name;//从路径获得文件名
	if (aName.empty())
	{
		file_name = CeUtility::GetFileNameFromPath(aPath);
	}
	else
	{
		file_name = aName;
	}

	//重复检测
	auto iter = mMaterials.find(file_name);
	if (iter != mMaterials.end())
	{
		//ReloadMaterial(file_name, aPath);
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name repeated, material reload." });
		return 0;//未来支持材质脚本重新编译，去掉这个。材质脚本重新编译需要记录引用该材质的游戏物体，并在脚本重新
		//编译之后更新引用材质的游戏物体的材质
	}

	//解析材质脚本所需的临时变量
	vector<ShaderPrototype> shader_pros;
	MaterialPrototype material_pros;
	string mat_script;
	//句柄临时变量
	Material mat; mat.Clear();
	//将数据存入map前创建的临时变量以避免错误返回

	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name ", CeUtility::GetFileNameFromPath(aPath) });
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material path ", aPath });
	//从材质文件中读取材质脚本
	mat_script = CeLoader::LoadText(aPath);
	if (mat_script.empty())
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Material file path exception. " });
		return 0;
	}
	int lex_tag =  CeMaterialLexer::LexicalAnalysis(mat_script, &shader_pros,&material_pros);//解析材质脚本
	if (!lex_tag)
	{
		DebugLogManager::DebugOutput("Resourcemanager", EDebugTag::EDT_Log, { CeMaterialLexer::GetDebugLog() });
		return 0;
	}
	
	for (auto val : shader_pros)
	{
		//处理纹理
		vector<unsigned> hds;//一个着色器的纹理
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

		//处理着色器
		unsigned shader =
			CeShaderCompiler::CompileShader(val.vertex, val.fragment, val.geometry);
		
		if (!shader)
		{			
			//DebugLogManager::DebugOutput("ResourceManger", EDebugTag::EDT_ERROR, {"Vertex code is: \n" ,val.vertex, "\n", "Fragment code is: \n"  ,val.fragment });
			CeTextureFormatter::DeleteTexture2D(mat.GetTextureHds());
			CeShaderCompiler::DeleteShaderProgram(mat.GetShaderHds());
			return 0;
		}		
		//绑定纹理	 
		

		mat.AddShader(shader);
		mat.AddShaderElement(shader, hds, val.texture_name);
		mat.SetEmissiveColor(material_pros.color[0], material_pros.color[1], material_pros.color[2]);	
		mat.SetPath(aPath);		
	}

	DebugLogManager::DebugOutput("ResourceManger", EDebugTag::EDT_Log, { "Material load success." });
	mMaterials.insert(pair<string, Material>(file_name, mat));//存入map中
	
	return 1;
}

/*==============================================================================================================*/

int ResourceManager::ReloadMaterial(const string& aName, const string& aPath)
{
	//先删除之前的，会导致之前的游戏物体渲染失效
	CeTextureFormatter::DeleteTexture2D(mMaterials[aName].GetTextureHds());
	CeShaderCompiler::DeleteShaderProgram(mMaterials[aName].GetShaderHds());


	//解析材质脚本所需的临时变量
	vector<string> tex_names;
	vector<string> tex_paths;
	ShaderCode code;
	string mat_script;
	//句柄临时变量
	Material mat;
	vector<unsigned> tex_hds;
	int state = 0;//这个状态标记，如果哪个阶段有错误（设置state<0）就执行释放函数


	//将数据存入map前创建的临时变量以避免错误返回

	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material name ", CeUtility::GetFileNameFromPath(aPath) });
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { "Material path ", aPath });
	//从材质文件中读取材质脚本
	mat_script = CeLoader::LoadText(aPath);
	if (mat_script.empty())
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Material file path exception. " });
		return 0;
	}
	CeMaterialLexer::LexicalAnalysis(mat_script, &tex_names, &tex_paths, &code);//解析材质脚本
	DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_Log, { CeMaterialLexer::GetDebugLog() });//这个是材质脚本解析log

	//加载纹理
	int i = 0;
	while (i < tex_names.size() && i < tex_paths.size())
	{
		tex_names[i];
		tex_paths[i];
		auto tex = LoadTexture2D(tex_paths[i]);//这里没有将生成的纹理2D存到map中
		if (!tex)
		{
			DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Texture file path exception. " });
			//释放之前加载的纹理
			CeTextureFormatter::DeleteTexture2D(tex_hds);
			return -2;
		}
		tex_hds.push_back(tex);
		i++;
	}

	//编译着色器	
	unsigned shader_hd =
		CeShaderCompiler::CompileShader(code.vertex.c_str(), code.fragment.c_str());
	if (!shader_hd)
	{
		DebugLogManager::DebugOutput<ResourceManager>(EDebugTag::EDT_ERROR, { "Shader compile exception." });
		//释放之前加载的纹理
		CeTextureFormatter::DeleteTexture2D(tex_hds);
		return -3;
	}

	//绑定材质到着色器
	CeShaderCompiler::SetTextureUnitName(shader_hd, tex_names);
	for (int i = 0; i < tex_hds.size(); i++)
	{
		CeTextureFormatter::BindTexture2D(shader_hd, tex_hds[i], i);
	}

	//将材质存储到map中
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
 * @brief 加载模型并存储
 * @TODO 自动提取纹理材质和名字，生成材质模板（纹理路径已经写好）
 */
int ResourceManager::LoadStaticMesh(const string& aPath)
{
	string file_name = CeUtility::GetFileNameFromPath(aPath);
	//重复检测
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
	//创建一个2D纹理并返回它的纹理信息，并使用默认环绕和过滤，纹理环绕和过滤可以在Texture类中修改
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





 
 