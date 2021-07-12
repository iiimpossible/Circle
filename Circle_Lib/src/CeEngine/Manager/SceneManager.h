#pragma once

#include <vector>
#include <string>
#include <Celib_x64/src/CeCommon/Tool/Delegate.h>
using std::vector;
using std::string;
using namespace CircleEngine;
 
class RenderManager;
class GameObject;
class Scene;
 

 

/**
* @brief 该类时管理当前的所有场景的管理器，其中保存一个场景指针指向当前活动场景
* @event 场景增删事件
* @event 游戏物体增删事件
* @event 
*
*/
class SceneManager
{
public:
	//构造函数
	SceneManager();
	~SceneManager();

public:
	/**
	 * @brief 遍历对象容器，更新信息，生成渲染命令
	 */
	void Update();

	void Start();
public:
	//场景管理

	/**
	 * @brief 创建一个场景并注册到场景容器中
	 * @detail 管理一个场景id，在给场景命名的时候会给该场景一个唯一的名字(Scene+id)
	 * 创建场景直接使用一个new
	 */	
	static Scene* CreateScene(const string& aName = "Scene");

	static void InsertScene(Scene* aScene);
	/**
	 * @brief 切换当前场景到目标场景,注意要更换游戏物体容器的引用！
	 */	
	static void SwitchCurrentScene();
	/**
	 * @brief 返回当前活动场景指针
	 */	
	static Scene* GetCurrentScene();
	/**
	 * @brief 从容器中删除一个场景
	 */	
	static void DeleteScene(const string& aName);
	/**
	 * @brief 根据名字查找一个场景
	 * TODO:未来将场景容器改为使用map来装，目前为vector
	 */
	static Scene* FindScene(const string& aName);
	

	/**
	 * @brief 这个函数应该传递2个函数，一个是物体名字，另一个是物体类型
	 */
	static GameObject* CreateGameObjecet(const string& aName);

	static GameObject* InsertGameObejct(GameObject* aObject);


public:
	//当前场景操作


public:
	//事件

	Delegate<int, int> OnObjectAdd;
	Delegate<int, int> OnObjectDelete;
	Delegate<int, int> OnSceneAdd;
	Delegate<int, int> OnSceneDelete;

private:
	//场景列表释放场景

	/** 释放所有场景*/
	void ReleaseSecenes();

private:

 
	//什么情况下该指针会会出现状况？
	//在该类中释放该指针导致类外部的指向该指针指向的内存指针成为野指针，所以在该类中不能释放当前指针
	///当前活动场景,不在这里分配内存
	static Scene* CurrentScene;
	///场景容器
	static vector<Scene*> Scenes;
	///场景唯一ID
	static unsigned int SceneID;
	///渲染调度器，越来越乱了
	 
};