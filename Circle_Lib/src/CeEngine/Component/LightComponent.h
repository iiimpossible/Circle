#pragma once
#include <string>
#include <Celib_x64/src/CeEngine/Component/Component.h>
#include <CeEngine/Object/GameObject.h>
using std::string;

 
class GameObject;
class MeshRenderer;
/**
 * @breif 将游戏物体变为一个光源
 * @detail 提供光照颜色、是否泛光、等。在构造函数中将游戏物体注册到renderer中，方便
 * 渲染器获取光源颜色和位置和其他一些特性
 */
class LightComponent:public Component
{
public:

	LightComponent(GameObject* aObject, const string& aName = "LightComponent");
	~LightComponent();

public:

	/** @breif 提供光源对象的光照信息，供光照管理器每帧调用*/
	const LightCommand& GetLightInfo();

	/** @breif 设置光源对象的着色器发光颜色*/
	void SetEmissiveColor(const Vec3& aColor);


	virtual void SetCommand(const string& aCmd)override;

public:

	virtual void Start()override;

	virtual void Update() override;

private:

	Mat4 GetLightSpaceMat();


private:

	/** @breif 临时的静态的光照命令*/
	LightCommand mLightCmd;//

	/** @breif 组件拥有者的网格渲染组件*/
	MeshRenderer* mOwnerMeshRenderer;
};

 