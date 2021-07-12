#include "Cube.h"
#include <Celib_x64/src/CeCommon/Common.h>
#include <Celib_x64/src/CeEngine/Component/MeshRenderer.h>
#include <Celib_x64/src/CeEngine/Render/Material.h>
#include <Celib_x64/src/CeEngine/Render/StaticMesh.h>
#include <Celib_x64/src/CeEngine/Manager/ResourceManager.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
Cube::Cube(const string& aName):
	GameObject(aName)
{	
	ResourceManager::LoadMaterial("asset/material/cube.material");
	auto material = ResourceManager::GetMaterial("cube.material");
	mMeshRender->SetMaterial(material);
	this->SetObejctPosition(Vec3(0.0f,0.0f,10.0f));

}

Cube::~Cube()
{
}



void Cube::Start()
{
	
}

 
void Cube::Update()
{	


	 
}

