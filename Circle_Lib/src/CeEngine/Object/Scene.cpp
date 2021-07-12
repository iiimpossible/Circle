#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"
#include <CeCommon/Tool/CeStructs.h>
#include <CeEngine/Engine.h>
#include <CeEngine/Manager/DebugLogManager.h>
#include <CeEngine/Component/SpriteRenderer.h>
#include <CeEngine/Component/CameraComponent.h>
#include <CeEngine/Render/RenderManager.h>
#include <CeEngine/Render/RenderCommand.h>
#include <CeEngine/Object/ObjectFactory.h>

Scene::Scene(const string& aName):
	Object(aName)
{
	mGameobjctID = 1;//���ڸ���Ϸ�����ŵ�

	bIsRunningMode = true;

	bTestDelete = false;

	mCameraIndex = 0;
}

 

Scene::~Scene()
{
	for (auto val : mGameobjects)
	{
		delete(val);
		val = nullptr;		
	}
	mGameobjects.clear();
}

  

GameObject* Scene::InsertGameObject(GameObject * aObject)
{	
	if (aObject)
	{
		if (!FindGameObject(aObject->GetName()))
		{
			mGameobjectsBuffer.push_back(aObject);
		}
		else
		{
			aObject->SetName(aObject->GetName() + std::to_string(mGameobjctID++));			
			//this->mGameobjects.push_back(aObject);
			mGameobjectsBuffer.push_back(aObject);
		}
		return aObject;
	}
	else
	{
		DebugLogManager::DebugOutput({ "@Scene: Inserted object invalid! " });		
		return nullptr;
	}	
}

void Scene::InsertGameObject(const string& aType)
{
	mNewObjects.emplace_back(aType);
}

GameObject* Scene::CreateGameObject(const string& aName)
{
	GameObject* obj = new GameObject(aName + to_string(mGameobjctID++));
	mGameobjectsBuffer.push_back(obj);
	//this->mGameobjects.push_back(obj);
	return obj;
}

void Scene::DeleteGameObject(const string& aName)
{		 
	for (auto iter = mGameobjects.begin(); iter != mGameobjects.end(); iter++)
	{		 
		GameObject* obj = (*iter);
		if ((*iter)->GetName() == aName)
		{			 
			obj->bIsDestroy = true;
		}		
	} 
}

void Scene::ClearScene()
{		
	for (auto iter = mGameobjects.begin(); iter!=mGameobjects.end();iter++)
	{
		(*iter)->bIsDestroy = true;
	}	 	
}

void Scene::SetGameobjctName(const string & aOldName, const string & aNewName)
{
	GameObject* obj = FindGameObject(aOldName);	
	if (!obj)
	{
		DebugLogManager::DebugOutput("Engind", EDebugTag::EDT_ERROR, { "Object not exist. Input name is: ", aOldName });
		return;
	}	
	GameObject* nametest_obj =  FindGameObject(aNewName);
	string name;
	if (!nametest_obj)
	{
		name = aNewName;
	}
	else
	{
		name = aNewName + to_string(mGameobjctID++);
	}
	obj->SetName(name);
	Engine::mOnGameobjectRename.Invoke(name);
}



vector<string> Scene::GetGameObjectNameList() const
{
	vector<string> temp_name;
	for (auto val : mGameobjects)
	{
		temp_name.push_back(val->GetName());
	}
	return temp_name;
}

 
GameObject * Scene::FindGameObject(const string& aName)
{ 		
	for (auto val : mGameobjects)
	{		
		if (val->GetName() == aName)
		{
			return val;
		}	
	}
	return nullptr;	
}

int Scene::GetGameObjectNum() const
{
	return (int)mGameobjects.size();
}

list<GameObject*>& Scene::GetGameObjects()
{
	return  mGameobjects;
}


void Scene::InsertCameraComponent(CameraComponent * aCameraComp)
{
	mCameraComps.push_back(aCameraComp);
}

void Scene::DeleteCameraComponent(CameraComponent * aCameraComp)
{	
	for (auto iter = mCameraComps.begin();iter != mCameraComps.end();iter++)
	{
		if ((*iter)==aCameraComp)
		{
			(*iter) = 0;
			iter = mCameraComps.erase(iter);
			break;
		}
	}
}

vector<ObjectInitInfo> Scene::GetObjectInitInfo()
{ 
	vector<ObjectInitInfo> infos;
	ObjectInitInfo temp_info;
	for (auto obj : mGameobjects)
	{
		if (!obj) continue;
		temp_info.name = obj->GetName();
		temp_info.type = obj->GetStaticClassName();
		temp_info.comp_command = obj->GetComponentInfo();
		infos.push_back(temp_info);
	}
	return infos;
}

void Scene::SetRunningMode(bool aBool)
{
	bIsRunningMode = aBool;
}


/*===========================================================================================================*/



void Scene::Start()
{
	for (GameObject* object : mGameobjects)
		if (object)object->Start();
}

void Scene::ProcessGameObjectBuffer()
{
	mSceneMutex.lock();
	if (mGameobjectsBuffer.empty())
	{
		mSceneMutex.unlock();//ע��ÿ�����ط�֧����Ҫ����������
		return;
	}

	for (GameObject* obj : mGameobjectsBuffer)
	{
		mGameobjects.push_back(obj);
	}
	mGameobjectsBuffer.clear();
	mSceneMutex.unlock();
	Engine::mOnSceneRefresh.Invoke(this->GetGameObjectNameList());
}


/**
 * @breif ע�������ʹ��erase֮�󣬷��ص���һ���Ѿ�++���ĵ�������
 * ��˲�����erase֮��++
 */
void Scene::ProcessGameObjectDestroy()
{
	bool tag = false;
	for (auto iter = mGameobjects.begin(); iter != mGameobjects.end();)
	{
		if ((*iter)->bIsDestroy == true)
		{
			tag = true;
			delete(*iter);
			(*iter) = 0;
			iter = mGameobjects.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	if (tag)
		Engine::mOnSceneRefresh.Invoke(this->GetGameObjectNameList());
}

void Scene::ProcessNewGameOjects()
{
	if (mNewObjects.empty()) return;
	for (auto type : mNewObjects)
	{
		GameObject* obj = ObjectFactory::GetObjectInstance(type);
		if(obj)	InsertGameObject(obj);
	}
	mNewObjects.clear();
}

void Scene::Update()
{
	ProcessGameObjectDestroy();//�ȴ���ɾ��
	ProcessGameObjectBuffer();	
	ProcessNewGameOjects();

	if (mGameobjects.empty())return;
	if (mCameraComps.size() > 0)
	{
		RenderManager::InsertCameraInfo({ mCameraComps[mCameraIndex]->GetObjectPosition(),
			mCameraComps[mCameraIndex]->GetProjectionMatrix(),
			mCameraComps[mCameraIndex]->GetViewMatrix() });
	}
	
	//���������е���Ϸ����
	for (GameObject* object : mGameobjects)
	{
		//�����ָ��
		if (object == nullptr)
		{
			continue;
		}
		//�������Ϸ���屻���Ϊ����
		if (object->bIsDestroy == true)
		{
			//TODO:һ��ʱ�����󣬽����ΪDestroy����������
			continue;
		}
		 
		//�������Ϸ���屻���Ϊ����
		if (object->bIsVisible == false)
		{
			continue;
		} 
			 
		//������Ϸ����
		object->Update();

		switch (Engine::GetEngineState())
		{
		case EEngineState::EES_2D:			
			break;
		case EEngineState::EES_3D:	
		case EEngineState::EES_ShaderDevelop:	
		{
			RenderManager::CreateRenderCommand3D(object);
			break;
		}		
		default:
			break;
		}
		
	} 	
	
}


