#pragma once
#include <list>
#include <Celib_x64/src/CeCommon/Common.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
using std::list;
 
 
class LightComponent;


/**
 * @breif ���չ����������������Ⱦ��ͨ�Ų����ݹ�������
 * @note ���չ�������Update��Ҫ����Ⱦ֮ǰ��������֮��ִ��
 */
class LightManager
{
public:

	LightManager();
	~LightManager();
public:

	void Start();

	void Update();
public:

	/** @breif ���ܵ��ⲿ�����������*/
	static void InsertCommand(const LightCommand& aCommand);

	/** @breif ע��������*/
	static void InsertLightComponent(LightComponent* aComponent);

	static void DeleteLightComponent(LightComponent* aComponent);
private:

	/** @breif ÿִ֡�иú������������еĹ��������������ѡȾ��Ϣ����Ⱦ��*/
	void InsertCommand();
private:

	//��Ϊ���ܾ�����Ҫɾ��Ԫ�أ�����ʹ��list
	static list<LightComponent*> mLightComponents;
};