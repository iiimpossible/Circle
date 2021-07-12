#include "SceneManager.h"
#include <Celib_x64/src/CeEngine/Object/Scene.h>
#include <vector>
using std::vector;

 
///当前活动场景,不在这里分配内存
Scene* SceneManager::CurrentScene = nullptr; 
///场景容器
vector<Scene*> SceneManager::Scenes;
///场景唯一ID
unsigned SceneManager::SceneID = 0 ;
///渲染调度器，越来越乱了
 


SceneManager::SceneManager()
{
	 
	 
	this->CurrentScene = CreateScene("MainScene");
	//this->m_currentGameObjects = this->m_currentScene->GetGameObjects();
	//this->m_renderScheduler = new RenderManager();
	 
}

SceneManager::~SceneManager()
{
	//直接释放在容器中的所有场景
	//current场景置位零
	CurrentScene = nullptr;

}

 
 

Scene* SceneManager::CreateScene(const string& aName)
{
	Scene* tScene = new Scene(aName + to_string(SceneID));
	Scenes.push_back(tScene);
	SceneID++;	
	return tScene;
}

void SceneManager::InsertScene(Scene * aScene)
{
	if (aScene->GetName().empty())
	{
		aScene->SetName("Scene_" + to_string(SceneID));
	}
	Scenes.push_back(aScene);
}

void SceneManager::SwitchCurrentScene()
{

}

Scene* SceneManager::GetCurrentScene()
{
	return CurrentScene;
}

void SceneManager::DeleteScene(const string& aName)
{
	for (auto iter = Scenes.begin(); iter != Scenes.end(); iter++)
	{
		 
		if ((*iter)->GetName() == aName)
		{
			delete (*iter);
			*iter = 0;
			Scenes.erase(iter);
			//m_Gameobjects.erase(iter);
			//好像迭代器丢失了引用导致bug
			break;
		}
	}
	 
}

Scene * SceneManager::FindScene(const string& aName)
{
	if (Scenes.empty()) return nullptr;

	for (auto val : Scenes)
	{
		if (val->GetName() == aName)
		{
			return val;
		}
	}
	return nullptr;
}

void SceneManager::Update()
{
	CurrentScene->Update();
}

void SceneManager::Start()
{
	CurrentScene->Start();
}

GameObject* SceneManager::CreateGameObjecet(const string& aName)
{
	return CurrentScene->CreateGameObject(aName);
}

GameObject * SceneManager::InsertGameObejct(GameObject* aGameobject)
{
	return CurrentScene->InsertGameObject(aGameobject);
}

 

