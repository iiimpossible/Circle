#pragma once
#include "RenderCommand.h"
#include <vector>
#include "Texture2D.h"
using std::vector;

struct GameObjectInfo
{
	Vec3 Position;//λ��
	Vec3 Rotation;//��ת
	Vec3 Size;//�ߴ�
};

struct MaterialInof
{
	//��ɫ��
	unsigned ShaderHandle;

	//������Ϣ
	float DiffuseStrength = 0.5f;//������ǿ��
	float ShinessStrength = 64.0f;//�����ǿ��
	//������
	vector<Texture2D> mTextures;
};

//�����λ�á���ת�ȼ�����Ϣ��͸�ӻ�������
struct CameraInfo
{
	Mat4 PojectionMat;//ͶӰ����
	Mat4 ViewMat;//�ӿھ���
	Vec3 Position;//���λ��
	Vec3 Rotation;//�����ת	
};

//3D �����������Ϣ�����񶥵����������������������������������������
struct MeshInfo
{	
	//������
	vector<unsigned> VertexArrayHandles;
	//vector<int> VertexNum;//�᲻����ģ�͵Ķ�����������32λ���͵����ֵ����ʮ���ڸ����㣬���ڸ�������
	vector<int> IndicesNum;
};

/**
 * @brief ��׼��3D��Ⱦ����
 * �����Ǵ���ʲô���������ԣ��������ԣ�������ԣ�ʹ�ÿ�����ֵ���ݻ���ʹ��ָ�룿
 * ��Ҫ��¼�Ƿ���2D����3D��
 * ������Ҫһ���������飨����������������һ��ɫ��
 * ������Ҫһ���������飨���ݶ�����������
 * ��ô��������ϢMaterialInfo  CameraInfo, MeshInofӦ�÷ŵ����
 * ��������Ⱦ������߶���һЩ���ԣ�Ȼ�󽫲��ʡ����������Ĺؼ����ݱ����
 * ��Դλ�á�����������ǿ�ȡ���������ô�����أ�
 * ��ЩӦ����Ҫ�ŵ���Ⱦ����������ȥ�ɣ���ʱ��֧��ֱ������Ϸ�����ϼ�һ�����������
 * ʹ������ɹ�Դ
 * ʵ���ϣ���Ⱦ�������������Ⱦ�����ʵ��ϸ�ڣ�ȷ�е�˵���⼸���ṹ�������
 * ���ڲ������������⣬һ��Ϊ�ɹ����з���
 */
class RenderCommand3D
{
public:
	RenderCommand3D(const GameObjectInfo& aGInfo,const MaterialInof& aMTInfo, const MeshInfo& aMSInfo, const CameraInfo& aCInfo);
	~RenderCommand3D();

	GameObjectInfo mGOInfo;
	CameraInfo mCInfo;
	MaterialInof mMTInfo;
	MeshInfo mMSInfo;
private:

};

 