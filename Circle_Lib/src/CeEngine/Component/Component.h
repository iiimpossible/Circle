#pragma once
#include <iostream>
#include <string>
#include "../Object/Object.h"
using std::string;
 


//class InputCommand
//{
//public:
//	string ComponentType;//根据这个找到组件类型
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
 * @brief 该类是组件类，赋予游戏物体类功能
 * @details  该类是所有的组件的基类，继承于Object
 * 核心

使用组件工厂来创建组件 利用反射
 */
class Component:public Object
{
public:
	 
	Component(GameObject* aOwner = nullptr, const string& aName = "Component");
	virtual ~Component();

public:
	//信息输入接口

	/**
	 * @breif 这个会定义一系列命令作为该方法的输入。一个状态机会识别场景脚本中的命令，并将其转化
	 * 为该方法能够识别的命令
	 */
	virtual void SetCommand(const string& aCmd) = 0;

	virtual string GetCommand();

public:

	/** @breif 组件的Update和渲染的Update不是一个系统，这个Update由特定的管理器调用*/
	virtual void Update() = 0;
	virtual void Start() = 0;
	//一个接口，定义组件的核心特殊功能，比如，碰撞组件就是碰撞算法，还得考虑考虑
	//virtual void SpecialFuntion() = 0;
	GameObject* GetOwner() const;
protected:
	GameObject* mOwner;
	string mCurrentCommand;
};








