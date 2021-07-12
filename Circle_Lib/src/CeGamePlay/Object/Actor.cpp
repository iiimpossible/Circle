#include "Actor.h"
#include <Celib_x64/src/CeEngine/Component/MeshRenderer.h>
#include <Celib_x64/src/CeEngine/Render/Material.h>
#include <Celib_x64/src/CeEngine/Render/StaticMesh.h>
#include <Celib_x64/src/CeEngine/Manager/ResourceManager.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>

Actor::Actor(const string& aName)
{
	//ResourceManager::LoadStaticMesh("asset/model/nanosuit/nanosuit.obj");
	ResourceManager::LoadStaticMesh("asset/model/bunny/bunny.obj");
	ResourceManager::LoadMaterial("asset/material/bunny.material");
	//if (ResourceManager::GetStaticMesh("nanosuit.obj").GetEBO())
	//{
	auto mesh = ResourceManager::GetStaticMesh("bunny.obj");
	auto material = ResourceManager::GetMaterial("bunny.material");
	mMeshRender->SetMaterial(material);
	//auto mesh = ResourceManager::GetStaticMesh("nanosuit.obj");
	mMeshRender->SetStaticMesh(mesh);
	//DebugLogManager::DebugOutput({ "@Cube LOG: GetMultiVBO().size() ", std::to_string(mesh.GetMultiVBO().size()) });
//	DebugLogManager::DebugOutput({ "@Cube LOG: GetMultiEBO().size() ", std::to_string(mesh.GetMultiEBO().size()) });
	//}	//

	this->SetObejctPosition(Vec3(0.0f, 0.0f, 10.0f));
	this->SetObjectSize(Vec3(0.01f, 0.01f, 0.01f));
}
Actor::~Actor()
{

}

 void Actor::Start()
{

}

 void Actor::Update()
{

}