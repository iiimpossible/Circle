#include "Sun.h"
#include <CeCommon/Common.h>
#include <CeEngine/Component/MeshRenderer.h>
#include <CeEngine/Render/Material.h>
#include <CeEngine/Render/StaticMesh.h>
#include <CeEngine/Manager/ResourceManager.h>
#include <CeEngine/Manager/DebugLogManager.h>
#include <CeEngine/Component/LightComponent.h>
Sun::Sun(const string & aName)
{
	mMeshRender->SetMaterial("asset/material/dirLight.material", aName + "dirLight.material");
	//�������ע����ɻ�������������ͷ�
	mLightComp = AddComponent<LightComponent>(new LightComponent(this));

	ResourceManager::LoadStaticMesh("asset/model/earth/earth.obj");
	auto mesh = ResourceManager::GetStaticMesh("earth.obj");
	mMeshRender->SetStaticMesh(mesh);
}

Sun::~Sun()
{

}

void Sun::Start()
{
}

void Sun::Update()
{
}
