#pragma once
#include <iostream>
#include <string>
#include "../Object/Object.h"
using std::string;
 


//class InputCommand
//{
//public:
//	string ComponentType;//��������ҵ��������
//public:
//	
//private:
//};
//
//class MoveCommand :public InputCommand
//{
//public:
//	Vec3 target;
//public:
//
//
//private:
//};
//
//
//class ColorCommand :public InputCommand
//{
//public:
//	Vec3 color;
//public:
//
//
//private:
//};





class GameObject;
class Texture2D;

 /**
 * @brief ����������࣬������Ϸ�����๦��
 * @details  ���������е�����Ļ��࣬�̳���Object
 * ����

ʹ������������������ ���÷���
 */
class Component:public Object
{
public:
	 
	Component(GameObject* aOwner = nullptr, const string& aName = "Component");
	virtual ~Component();

public:
	//��Ϣ����ӿ�

	/**
	 * @breif ����ᶨ��һϵ��������Ϊ�÷��������롣һ��״̬����ʶ�𳡾��ű��е����������ת��
	 * Ϊ�÷����ܹ�ʶ�������
	 */
	virtual void SetCommand(const string& aCmd) = 0;

	virtual string GetCommand();

public:

	/** @breif �����Update����Ⱦ��Update����һ��ϵͳ�����Update���ض��Ĺ���������*/
	virtual void Update() = 0;
	virtual void Start() = 0;
	//һ���ӿڣ���������ĺ������⹦�ܣ����磬��ײ���������ײ�㷨�����ÿ��ǿ���
	//virtual void SpecialFuntion() = 0;
	GameObject* GetOwner() const;
protected:
	GameObject* mOwner;
	string mCurrentCommand;
};








