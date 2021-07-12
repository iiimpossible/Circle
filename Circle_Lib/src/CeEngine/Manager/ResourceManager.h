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
	//��ʼ��
	void Start();


public:
	//����

	/** �������ʲ�������Ӧ��Դ*/
	static int LoadMaterialOld(const string& aPath);

	static int LoadMaterial(const string & aPath, const string& aName = "");

	/** ��������ֻ�ṩ����������ʣ�*/
	static unsigned LoadTexture2D(const string& aPath);

	static unsigned LoadTextureCube(const string& aPath);

	/** ����ͼƬ������ΪTexture�������������ִ���map��*/
	static Texture2D LoadTexture(const char* file, string aName);

	/** ���ı��ļ��м�����ɫ�����������������Ҫ��*/
	static Shader LoadShader(const char* aVShader, const char*aFShader, const char* aGShader, string aName) = delete;

	/** ����ģ�Ͳ������ִ�����map��*/
	static int LoadStaticMesh(const string& aPath);

	 
public:
	//���

	static Material& GetMaterial(const string& aName);	
	/** �������з��ظ���������*/
	static Texture2D& GetTexture(string aName);	
	/** �������з��ظ���ɫ������*/
	static Shader& GetShader(string aName);	

	static StaticMesh& GetStaticMesh(const string& aName);	

	static vector<string> GetMaterialList();
public:
	//����

	static bool FindMaterial(const string& aName);
private:
	//

	/** �����ؼ��ء������Ǳ���ɾ����֮��Ӧ���ȼ���޸�����Щ���ݣ�Ȼ���ٿ����Ƿ�ɾ���������ɫ��*/
	static int ReloadMaterial(const string& aName, const string& aPath);
private:
	///��������
	static map<string, Texture2D> mTextures;
	///��ɫ������
	static map<string, Shader> mShaders;

	static map<string, StaticMesh> mStaticMeshes;

	static unordered_map<string, Material> mMaterials;

	static unordered_map<string, unsigned> mTextureCubes;
	///
	//static map<unsigned char, Character> characters;
	/**
	 * @brief TODO:����������Ҫ������ȥ����Ϊһ����ȡ���ݵ���
	 */
	static Texture2D LoadTextureFromFile(const char* file);
	
	
};