#pragma once
#include <vector>
#include <Celib_x64/src/CeCommon/Common.h>
using std::vector;
using namespace CircleEngine;
 
class Camera;
class GameObject; 

/**
* @brief 渲染命令，存储渲染相关信息
* @details 渲染需要的数据：着色器id，纹理id，网格的VAOid
* TODO:将摄像机位置什么的去掉，直接传矩阵4*4 =16B，传向量只要6B确实带宽消耗更多
*/
class RenderCommand 	 
{
public:	
	/**
	 * @brief 构造函数，打包游戏物体的渲染数据
	 */
	//RenderCommand();
	/**
	 * @brief 构造函数，打包游戏物体的渲染数据
	 * @note 这里的数据类型其实容易混淆，还是需要独立一个类或者结构体来作为参数，否则
	 * 参数输入错误就完了
	 * TODO:封装这些数据类，位置，旋转，规模
	 */
	//这个构造函数在RenderSchedule类中被调用，预备废除
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

