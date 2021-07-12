#pragma once

#include <vector>
#include <list>
#include <string>
#include <mutex>
#include "Object.h"
using namespace std;

#include <Celib_x64/src/CeCommon/Common.h>
class GameObject;
class Camera;
class CameraComponent;

/**
 * @breif 场景类，管理场景中所有的游戏物体
 */
class Scene :public Object
{
public:
 
	Scene(const string& aName = "Scene");
	Scene(Scene& aName) = delete;
	~Scene();
public:
	//插入、创建物体

	/** @breif 插入游戏物体*/
	GameObject* InsertGameObject(GameObject* aObject);

	void InsertGameObject(const string& aType);
	
	/** @breif 创建一个游戏物体到场景中*/
	GameObject* CreateGameObject(const string& aName);
	 
	/** @breif 获得当前场景游戏物体数量*/
	int GetGameObjectNum() const;
	 
	/** @breif 获得游戏物体容器指针*/
	list<GameObject*>& GetGameObjects();	

	/** @breif 获取场景保存必要信息*/
	vector<ObjectInitInfo> GetObjectInitInfo();
public:
	//删除物体

	/** @breif 从当前场景中删除一个游戏物体*/
	void DeleteGameObject(const string& aName);

	/** @breif 清除场景内的所有物体*/
	void ClearScene();

public:
	//修改物体
	void SetGameobjctName(const string& aOldName, const string& aNewName);


public:
	//查找物体

		/** @breif 从当前场景中查找游戏物体*/
	GameObject* FindGameObject(const string& aName);
public:
	//相机操作

	/** @breif 获得当前相机*/
	//Camera* GetCurrentCamera();

	/** @breif 设定当前相机*/
	//void SetCurrentCamera(Camera* aCamera);

	void InsertCameraComponent(CameraComponent* aCameraComp);

	void DeleteCameraComponent(CameraComponent* aCameraComp);

	

public:
	//状态控制

	void SetRunningMode(bool aBool);	
public:

	/** @breif 游戏开始时执行一次*/
	void Start();
	 
	/** @breif 遍历游戏对象并更新它们*/
	void Update();
public:
	// 场景信息提取
	vector<string> GetGameObjectNameList() const ;

private:
	//游戏物体缓冲操作

	void ProcessGameObjectBuffer();

	void ProcessGameObjectDestroy();

	void ProcessNewGameOjects();
private:
	
	bool bIsRunningMode;

	bool bTestDelete;

	//游戏物体容器
	list<GameObject*> mGameobjects;
	//多线程缓冲，qt事件循环操作此缓冲，每帧开始会处理这个缓冲以加入游戏物体队列
	vector<GameObject*> mGameobjectsBuffer;

	vector<GameObject*> mGameObjectsDesytoyed;
	//游戏物体id，用于给游戏物体编号
	 unsigned int mGameobjctID;

	////相机
	//Camera* mCurrentCamera;

	///** 存储当前场景相机，相机会同时的引用会同时存在游戏物体列表和这里，有必要吗？*/
	//vector<Camera*> mCameras;

	unsigned mCameraIndex;
	vector<CameraComponent*> mCameraComps;

	vector<string> mNewObjects;

	mutex mSceneMutex;
};

