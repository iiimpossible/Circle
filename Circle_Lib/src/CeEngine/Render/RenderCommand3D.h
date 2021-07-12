#pragma once
#include "RenderCommand.h"
#include <vector>
#include "Texture2D.h"
using std::vector;

struct GameObjectInfo
{
	Vec3 Position;//位置
	Vec3 Rotation;//旋转
	Vec3 Size;//尺寸
};

struct MaterialInof
{
	//着色器
	unsigned ShaderHandle;

	//光照信息
	float DiffuseStrength = 0.5f;//漫反射强度
	float ShinessStrength = 64.0f;//镜面光强度
	//多纹理
	vector<Texture2D> mTextures;
};

//相机的位置、旋转等几何信息，透视或者正交
struct CameraInfo
{
	Mat4 PojectionMat;//投影矩阵
	Mat4 ViewMat;//视口矩阵
	Vec3 Position;//相机位置
	Vec3 Rotation;//相机旋转	
};

//3D 物体的网格信息，网格顶点数量，网格数量，顶点数组句柄，顶点索引句柄
struct MeshInfo
{	
	//多网格
	vector<unsigned> VertexArrayHandles;
	//vector<int> VertexNum;//会不会有模型的顶点数量超过32位整型的最大值？二十多亿个顶点，几亿个三角形
	vector<int> IndicesNum;
};

/**
 * @brief 标准的3D渲染命令
 * 参数是传递什么？材质属性？网格属性？相机属性？使用拷贝赋值传递还是使用指针？
 * 需要记录是否是2D还是3D？
 * 材质需要一个纹理数组（传递纹理句柄），单一着色器
 * 网格需要一个网格数组（传递顶点数组句柄）
 * 那么，材质信息MaterialInfo  CameraInfo, MeshInof应该放到哪里？
 * 还是在渲染明林里边定义一些属性，然后将材质、相机、网格的关键数据保存吧
 * 光源位置、数量、光照强度、类型又怎么处理呢？
 * 这些应该是要放到渲染器的容器中去吧，暂时不支持直接在游戏物体上加一个光照组件来
 * 使得它变成光源
 * 实际上，渲染器很依赖这个渲染命令的实现细节，确切的说是这几个结构体的属性
 * 现在不考虑性能问题，一切为成功运行服务
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

 