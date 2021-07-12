#pragma once
#include <vector>
#include <string>
#include <Celib_x64/src/CeCommon/Math/Math.h>
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
using std::vector;
using std::string;

/**
 * @brief ������ǹ��������ļ����ص��࣬�����ı���ͼ��ģ��
 * @detail ���������ָ�����ı���������ɫ��������ͼ���Դ棬����ģ�͵��Դ�
 * δ������������Ͷ�ȡ�Զ�����ļ������ͼ�ļ��������ļ�
 * @note ע���ļ�����·��������Ϊͷ�ļ�������Դ�ļ����·�����Ϊ�����ļ�Ŀ¼
 */
class CeLoader
{	
public:
	//ģ�ͼ���

	/**
	 * @brief ʹ��assimp�����ģ�ͣ�ֻ�������񲻼�����������ʹ�ò����ж����·������
	 * @param aPath ģ��·��
	 */
	static vector<CeMesh>& LoadModel(const string& aPath);

public:
	//ͼƬ����

	/**
	 * @brief ������غ���
	 * @param aPath ����·��
	 * @param aWidth ����X����������Ŀ��ָ�����
	 * @param aheight ����Y����������Ŀ��ָ�����
	 * @param aChannel ����ͨ����Ŀ��ָ�����
	 * @param aReqComp ����
	 * @note �÷������ص�unsigned char* ��һ����ڴ棬ע���ͷ�
	 */	 
	static unsigned char* LoadImageData(const char * aPath, int* aWidth, int* aheight, int* aChannel, int aReqComp = 0);

	/**
	 * @breif  ������պ���
	 * @note ��պ��������������淶��һ��Ҫ�������·����߼��ϡ��»���+��λ�������һ����׺��_right.jpg��
	 * �����·����׺����Ҫ�ӣ��ú������Զ����	 
	 * @detail ����Ŀ��	��λ
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_X	��
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_X	��
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Y	��
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	��
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Z	��
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	ǰ
	 */	 
	static vector<unsigned char*> LoadCubeImageData(const string& aPath, int* aWidth, int* aHeight, int* aChannel, int aReqComp = 0);

	/** �������������������ͷ��ڴ�*/
	static void FreeImageData(unsigned char * aData);

	/** @breif �ͷ���պ��ӵļ�������*/
	static void FreeCubeImageData(vector<unsigned char*> aData);

public:
	//�ı�����

	static string LoadText(const string& aPath);

	static string GetFileNameFromPath(const string& aPath);
public:

	/**
	 * @brief ���ز����ļ�����ȡ�����е���Ϣ
	 * @param aPaht �����ļ�·��
	 * @param aTexName �����������飬ָ�����
	 * @param aTexPath ����·����ָ�����
	 * @param aShaderCode ��ɫ�����룬ָ�����
	 */
	static void LoadMaterial(const char* aPath, 
		vector<string>* aTexName, 
		vector<string>* aTexPath, 
		vector<string>* aShaderCode);


private:

	/** @brief ��Assimp�ڵ�洢���������Ϊһ������������*/
	static CeMesh ProcessMesh(struct aiMesh* aMehs, const struct aiScene* aScene);

	/** @brief ʹ�ü�������ĺ�����ģ���ж�ȡ����*/
	//static void LoadMaterialTextures(struct aiMaterial *mat, enum aiTextureType type, const char* typeName);

	/** @brief ����Assimp�Ľڵ��������ͣ��ݹ���ʽ�ش���*/
	static void ProcessNode(struct aiNode* aNode, const struct  aiScene* aScene);

	/** ÿ�μ���ǰ������*/
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