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

	if(mOwner)//�洢�����ӵ���ߵ���Ⱦ���
		mOwnerMeshRenderer = mOwner->GetComponent<MeshRenderer>();


	//��ʼ�����������ɫ
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
�޷�������ı�ƹ�ķ��������ɫ������һ�ֵƹ⣬����������һ���ģ�����ɫ��һ������
��ô�����е���Ⱦ�ܹ��£�������ɫ��������Ⱦ���룩����ô���ֱܷ������ĸ��ƹ�Ĺ�����ɫ��
��������ɫ����һ����
*/
const LightCommand& LightComponent::GetLightInfo()
{	
	if (mOwner)
	{		
		if (mOwnerMeshRenderer)//�Ӳ����л�ȡ������ɫ
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
	 
	//��Ҫ������תΪ��ɫֵ
	
	if (mOwnerMeshRenderer)//�Ӳ����л�ȡ������ɫ
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
