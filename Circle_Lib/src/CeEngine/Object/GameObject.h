#pragma once
#include "SceneObject.h"
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
#include <Celib_x64/src/CeEngine/Object/ObjectFactory.h>

class MeshRenderer;
 /**
 * @breif 游戏物体类，该类是所有游戏性物体的基类
 * @detail 该类是3D游戏物体基类。具有一个MeshRender组件，该组件包含网格和材质，即
 * 渲染所需的必要信息
 * @TODO:关于组件垃圾回收，应该是在SceneObject基类中有一个组件引用存储，当执行Object的析构函数的时候，
 * 将所有的组件析构
 */
class GameObject: public SceneObject
{
public: 

	GameObject(const string& aName = "GameObject");
	GameObject(GameObject& aGObject) = delete;
	GameObject& operator=(GameObject& aGObject) = delete; 
	virtual ~GameObject();
public:

	void SetMaterial(const string& aMaterailName);	

	//一个纯虚函数用于获得渲染所需的相关信息，包括着色器、纹理、顶点数组
	RenderInfo GetRenderInfo() const;

	
public:

	virtual void Start() override;
	virtual void Update() override;
protected:
	 
	//要将基类的渲染信息提供功能收回来，不再兼容2D游戏物体了，2D游戏物体可以用一个特殊的不能Z向移动的相机
	//和2D游戏物体，不过可能性能很低。
	MeshRenderer* mMeshRender; 
};




