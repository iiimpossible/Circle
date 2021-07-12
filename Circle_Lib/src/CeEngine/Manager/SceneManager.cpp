#include "SceneManager.h"
#include <Celib_x64/src/CeEngine/Object/Scene.h>
#include <vector>
using std::vector;

 
///��ǰ�����,������������ڴ�
Scene* SceneManager::CurrentScene = nullptr; 
///��������
vector<Scene*> SceneManager::Scenes;
///����ΨһID
unsigned SceneManager::SceneID = 0 ;
///��Ⱦ��������Խ��Խ����
 


SceneManager::SceneManager()
{
	 
	 
	this->CurrentScene = CreateScene("MainScene");
	//this->m_currentGameObjects = this->m_currentScene->GetGameObjects();
	//this->m_renderScheduler = new RenderManager();
	 
}

SceneManager::~SceneManager()
{
	//ֱ���ͷ��������е����г���
	//current������λ��
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
			//�����������ʧ�����õ���bug
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

 

