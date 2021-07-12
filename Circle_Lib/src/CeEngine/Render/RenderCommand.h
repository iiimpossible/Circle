#pragma once
#include <vector>
#include <Celib_x64/src/CeCommon/Common.h>
using std::vector;
using namespace CircleEngine;
 
class Camera;
class GameObject; 

/**
* @brief ��Ⱦ����洢��Ⱦ�����Ϣ
* @details ��Ⱦ��Ҫ�����ݣ���ɫ��id������id�������VAOid
* TODO:�������λ��ʲô��ȥ����ֱ�Ӵ�����4*4 =16B��������ֻҪ6Bȷʵ�������ĸ���
*/
class RenderCommand 	 
{
public:	
	/**
	 * @brief ���캯���������Ϸ�������Ⱦ����
	 */
	//RenderCommand();
	/**
	 * @brief ���캯���������Ϸ�������Ⱦ����
	 * @note ���������������ʵ���׻�����������Ҫ����һ������߽ṹ������Ϊ����������
	 * ����������������
	 * TODO:��װ��Щ�����࣬λ�ã���ת����ģ
	 */
	//������캯����RenderSchedule���б����ã�Ԥ���ϳ�
	RenderCommand(GameObject* aObject = nullptr);
	//RnederCommand(const ObjectInfo& aObjectInfo, const RenderInfo& aRenderInfo, const CameraInfo& aCameraInfo);
	//RenderCommand(RenderCommand& aRenderCommand) = delete;
	RenderCommand& operator=(RenderCommand& aRenderCommand) = delete;

public:
	//unsigned int mShaderHD;
	//vector<unsigned> mShaderHDs;
	vector<unsigned> mVAOIDs;
	vector<unsigned> mEBOs;
	vector<unsigned> mIndicesNum;
	vector<ShaderElement> mshaderElements;

	Vec3 mPos;
	Vec3 mRotation;
	Vec3 mSize;

	bool bIgnore;
};

class RenderCommand2D
{
public:
	RenderCommand2D();
	~RenderCommand2D();
	RenderCommand2D(RenderCommand2D& aRenderCommand) = delete;
	RenderCommand2D& operator=(RenderCommand2D& aRenderCommand) = delete;
public:
	unsigned int mShaderHandle;
	unsigned int mSpriteHandle;
	unsigned int mVAOHandle;
	Vec2 mPosition;
	Vec2 mRotation;
	Vec2 mSize;	
	Mat4 mProjectionMat;
	Mat4 mViewMat;
private:

};

