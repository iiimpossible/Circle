#include "SceneObject.h"
#include <Celib_x64/src/CeEngine/Component/Transform.h>
#include "../Component/Component.h"
#include "../Manager/DebugLogManager.h"
#include <iostream>

SceneObject::SceneObject(const string& aName):
	Object(aName)
{
	mTransformComp = AddComponent<Transform>(new Transform());
	this->bIsDestroy = false;
	this->bIsVisible = true;
}

 

SceneObject::~SceneObject()
{
	//遍历映射释放所有的组件
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		delete (*iter).second; 
			(*iter).second = 0;
	}	 
	mComponents.clear();
}


 

 

vector<string> SceneObject::GetComponentList() const
{
	vector<string> components;
	auto iter = mComponents.begin();
	while (iter != mComponents.end())
	{
		components.push_back((*iter).first);
		iter++;
	}
	return components;
}

Component * SceneObject::GetComponent(const string& aCompType)
{
	auto iter = mComponents.find(aCompType);
	if (iter != mComponents.end())
	{
		return (*iter).second;
	}
	return nullptr;
}

vector<pair<string, string>> SceneObject::GetComponentInfo() const
{
	vector<pair<string, string>> infos;
	pair<string, string> temp_info;
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		temp_info.first = (*iter).second->GetStaticClassName();
		temp_info.second = (*iter).second->GetCommand();
		infos.push_back(temp_info);
	}	 
	return infos;
}

Vec3 SceneObject::GetObjectPosition() const
{	
	return mTransformComp->GetPosition();
}

Vec3 SceneObject::GetObjectSize()const
{
	return mTransformComp->GetSize();
}

Vec3 SceneObject::GetObjectRotation()const
{	
	return mTransformComp->GetRotation();
}

void SceneObject::SetObejctPosition(const Vec3& position)
{
	mTransformComp->SetPosition(position);
}

void SceneObject::SetObejctPosition(float x, float y, float z)
{
	mTransformComp->SetPosition(Vec3(x,y,z));
}

void SceneObject::SetObjectSize(const Vec3& size)
{
	mTransformComp->SetSize(size);
}

void SceneObject::SetObjectSize(float x, float y, float z)
{
	mTransformComp->SetSize(Vec3(x, y, z));
}

void SceneObject::SetObjectRotation(const Vec3& rotation)
{
	mTransformComp->SetRotation(rotation);
}

void SceneObject::SetObjectRotation(float x, float y, float z)
{
	mTransformComp->SetRotation(Vec3(x, y, z));
}

void SceneObject::AddObjectMovement(const Vec3& delta)
{
	mTransformComp->AddObjectMovement(delta);
	//DebugLogManager::DebugOutputVec3("Transform",  delta);
}

void SceneObject::AddObjectRotation(const Vec3 & aDelta)
{
	mTransformComp->AddObjectRotation(aDelta);
}

void SceneObject::Start()
{
}

void SceneObject::Update()
{
}

