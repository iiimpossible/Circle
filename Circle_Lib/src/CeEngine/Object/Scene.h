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
 * @breif �����࣬�����������е���Ϸ����
 */
class Scene :public Object
{
public:
 
	Scene(const string& aName = "Scene");
	Scene(Scene& aName) = delete;
	~Scene();
public:
	//���롢��������

	/** @breif ������Ϸ����*/
	GameObject* InsertGameObject(GameObject* aObject);

	void InsertGameObject(const string& aType);
	
	/** @breif ����һ����Ϸ���嵽������*/
	GameObject* CreateGameObject(const string& aName);
	 
	/** @breif ��õ�ǰ������Ϸ��������*/
	int GetGameObjectNum() const;
	 
	/** @breif �����Ϸ��������ָ��*/
	list<GameObject*>& GetGameObjects();	

	/** @breif ��ȡ���������Ҫ��Ϣ*/
	vector<ObjectInitInfo> GetObjectInitInfo();
public:
	//ɾ������

	/** @breif �ӵ�ǰ������ɾ��һ����Ϸ����*/
	void DeleteGameObject(const string& aName);

	/** @breif ��������ڵ���������*/
	void ClearScene();

public:
	//�޸�����
	void SetGameobjctName(const string& aOldName, const string& aNewName);


public:
	//��������

		/** @breif �ӵ�ǰ�����в�����Ϸ����*/
	GameObject* FindGameObject(const string& aName);
public:
	//�������

	/** @breif ��õ�ǰ���*/
	//Camera* GetCurrentCamera();

	/** @breif �趨��ǰ���*/
	//void SetCurrentCamera(Camera* aCamera);

	void InsertCameraComponent(CameraComponent* aCameraComp);

	void DeleteCameraComponent(CameraComponent* aCameraComp);

	

public:
	//״̬����

	void SetRunningMode(bool aBool);	
public:

	/** @breif ��Ϸ��ʼʱִ��һ��*/
	void Start();
	 
	/** @breif ������Ϸ���󲢸�������*/
	void Update();
public:
	// ������Ϣ��ȡ
	vector<string> GetGameObjectNameList() const ;

private:
	//��Ϸ���建�����

	void ProcessGameObjectBuffer();

	void ProcessGameObjectDestroy();

	void ProcessNewGameOjects();
private:
	
	bool bIsRunningMode;

	bool bTestDelete;

	//��Ϸ��������
	list<GameObject*> mGameobjects;
	//���̻߳��壬qt�¼�ѭ�������˻��壬ÿ֡��ʼ�ᴦ����������Լ�����Ϸ�������
	vector<GameObject*> mGameobjectsBuffer;

	vector<GameObject*> mGameObjectsDesytoyed;
	//��Ϸ����id�����ڸ���Ϸ������
	 unsigned int mGameobjctID;

	////���
	//Camera* mCurrentCamera;

	///** �洢��ǰ��������������ͬʱ�����û�ͬʱ������Ϸ�����б������б�Ҫ��*/
	//vector<Camera*> mCameras;

	unsigned mCameraIndex;
	vector<CameraComponent*> mCameraComps;

	vector<string> mNewObjects;

	mutex mSceneMutex;
};

