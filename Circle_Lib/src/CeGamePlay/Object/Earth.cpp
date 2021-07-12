#include "Earth.h"
#include <CeCommon/Common.h>
#include <CeCommon/Tool/Time.h>
#include <CeEngine/Component/MeshRenderer.h>
#include <CeEngine/Render/Material.h>
#include <CeEngine/Render/StaticMesh.h>
#include <CeEngine/Manager/ResourceManager.h>
#include <CeEngine/Manager/DebugLogManager.h>
Earth::Earth(const string & aName):
	GameObject(aName)
{
	ResourceManager::LoadStaticMesh("asset/model/earth/earth.obj");
	auto mesh = ResourceManager::GetStaticMesh("earth.obj");
	mMeshRender->SetStaticMesh(mesh);


	ResourceManager::LoadMaterial("asset/material/earth.material");
	auto material = ResourceManager::GetMaterial("earth.material");
	mMeshRender->SetMaterial(material);

}

Earth::~Earth()
{
}

void Earth::Start()
{
}

void Earth::Update()
{
	//自转
	AddObjectRotation(Vec3(0.0f, 0.1f, 0.0f));
	//公转
	RevolveAroundSun(Vec3(0.0f, 0.0f, 100.0f));
}

void Earth::RevolveAroundSun(const Vec3 & aCenter, float aRadius, float aAngularV)
{
	float x = aRadius * cos(0.1f * CeTimer::GetTime()) + aCenter.x;
	float y = aCenter.y;
	float z = aRadius * sin(0.1f * CeTimer::GetTime()) + aCenter.z;
	Vec3 pos = Vec3(x, y, z);
	SetObejctPosition(pos);
}

