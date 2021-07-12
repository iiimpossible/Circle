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
* @brief ����ʱ����ǰ�����г����Ĺ����������б���һ������ָ��ָ��ǰ�����
* @event ������ɾ�¼�
* @event ��Ϸ������ɾ�¼�
* @event 
*
*/
class SceneManager
{
public:
	//���캯��
	SceneManager();
	~SceneManager();

public:
	/**
	 * @brief ��������������������Ϣ��������Ⱦ����
	 */
	void Update();

	void Start();
public:
	//��������

	/**
	 * @brief ����һ��������ע�ᵽ����������
	 * @detail ����һ������id���ڸ�����������ʱ�����ó���һ��Ψһ������(Scene+id)
	 * ��������ֱ��ʹ��һ��new
	 */	
	static Scene* CreateScene(const string& aName = "Scene");

	static void InsertScene(Scene* aScene);
	/**
	 * @brief �л���ǰ������Ŀ�곡��,ע��Ҫ������Ϸ�������������ã�
	 */	
	static void SwitchCurrentScene();
	/**
	 * @brief ���ص�ǰ�����ָ��
	 */	
	static Scene* GetCurrentScene();
	/**
	 * @brief ��������ɾ��һ������
	 */	
	static void DeleteScene(const string& aName);
	/**
	 * @brief �������ֲ���һ������
	 * TODO:δ��������������Ϊʹ��map��װ��ĿǰΪvector
	 */
	static Scene* FindScene(const string& aName);
	

	/**
	 * @brief �������Ӧ�ô���2��������һ�����������֣���һ������������
	 */
	static GameObject* CreateGameObjecet(const string& aName);

	static GameObject* InsertGameObejct(GameObject* aObject);


public:
	//��ǰ��������


public:
	//�¼�

	Delegate<int, int> OnObjectAdd;
	Delegate<int, int> OnObjectDelete;
	Delegate<int, int> OnSceneAdd;
	Delegate<int, int> OnSceneDelete;

private:
	//�����б��ͷų���

	/** �ͷ����г���*/
	void ReleaseSecenes();

private:

 
	//ʲô����¸�ָ�������״����
	//�ڸ������ͷŸ�ָ�뵼�����ⲿ��ָ���ָ��ָ����ڴ�ָ���ΪҰָ�룬�����ڸ����в����ͷŵ�ǰָ��
	///��ǰ�����,������������ڴ�
	static Scene* CurrentScene;
	///��������
	static vector<Scene*> Scenes;
	///����ΨһID
	static unsigned int SceneID;
	///��Ⱦ��������Խ��Խ����
	 
};