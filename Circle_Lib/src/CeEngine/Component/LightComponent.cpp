#include "LightComponent.h"
#include <CeEngine/Object/GameObject.h>
#include <CeEngine/Render/Material.h>
#include <CeEngine/Component/MeshRenderer.h>
#include <CeEngine/Manager/LightManager.h>
#include <CeCommon/OpenGL/CeShaderCompiler.h>
#include <CeCommon/Tool/CeVM.h>

LightComponent::LightComponent(GameObject * aObject, const string & aName):
	Component(aObject,aName)
{
	mLightCmd.lightColor = Vec3(1.0f);
	mLightCmd.lightPos = Vec3(0.0f);
	LightManager::InsertLightComponent(this);

	if(mOwner)//存储该组件拥有者的渲染组件
		mOwnerMeshRenderer = mOwner->GetComponent<MeshRenderer>();


	//初始化发光物的颜色
	if (mOwnerMeshRenderer)
	{
		mOwnerMeshRenderer->GetMaterail().SetEmissiveColor(Vec3(1.0f));
	}	
}

LightComponent::~LightComponent()
{
	LightManager::DeleteLightComponent(this);
}

/*
无法在这里改变灯光的发光材质颜色。对于一种灯光，如果其材质是一样的（即着色器一样），
那么在现有的渲染架构下（光照颜色与网格渲染分离），那么不能分别设置哪个灯光的光照颜色，
除非其着色器不一样。
*/
const LightCommand& LightComponent::GetLightInfo()
{	
	if (mOwner)
	{		
		if (mOwnerMeshRenderer)//从材质中获取发光颜色
			mLightCmd.lightColor = mOwnerMeshRenderer->GetMaterail().GetEmissiveColor();
		
		mLightCmd.lightPos = mOwner->GetObjectPosition();

		mLightCmd.lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);// glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f); //
		mLightCmd.lightView = glm::lookAt(mOwner->GetObjectPosition(),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//mLightCmd.lightSpaceMat =  *
			
	}	
	return mLightCmd;
}

void LightComponent::SetEmissiveColor(const Vec3 & aColor)
{
	if (!mOwnerMeshRenderer) return;
	Material mat = mOwnerMeshRenderer->GetMaterail();	
	mat.SetEmissiveColor(aColor);
}
void LightComponent::SetCommand(const string & aCmd)
{
	if (aCmd.empty()) return;
	 
	//需要将输入转为颜色值
	
	if (mOwnerMeshRenderer)//从材质中获取发光颜色
		mOwnerMeshRenderer->GetMaterail().SetEmissiveColor(CeVM::StrToVec3(aCmd));	
	mCurrentCommand = aCmd;
}


void LightComponent::Start()
{
}

void LightComponent::Update()
{
}

Mat4 LightComponent::GetLightSpaceMat()
{


	return Mat4();
}
