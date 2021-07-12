#include "DirLight.h"
#include <CeEngine/Component/LightComponent.h>
#include <CeEngine/Manager/DebugLogManager.h>
#include <CeEngine/Component/MeshRenderer.h>
#include <CeEngine/Render/Material.h>
#include <CeCommon/OpenGL/CeShaderCompiler.h>
#include <CeCommon/Tool/Time.h>

DirLight::DirLight(const string& aName) :
	GameObject(aName)
{	
	//��仰������ǰ��
	mMeshRender->SetMaterial("asset/material/dirLight.material", aName+"dirLight.material");
	//�����ϱ߸ı�Ĳ����ڹ��캯�������÷�����ɫ
	mLightComp = AddComponent<LightComponent>(new LightComponent(this));
	
}

DirLight::~DirLight()
{
}



void DirLight::Start()
{
}

void DirLight::Update()
{
	//SetObejctPosition(30.0f*cos(CeTimer::GetTime()*0.1f), 30.0f*sin(CeTimer::GetTime()*0.1f), 0.0f);
}
