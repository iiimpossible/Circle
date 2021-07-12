#pragma once

#include <string>
#include <map>
#include "Object.h"
#include <Celib_x64/src/CeCommon/Common.h>
using namespace CircleEngine;
using std::string;
using std::map;

class Transform;
class Component;
 /**
 * @brief 类的简单概述
 * @details 类的详细概述
 */
class SceneObject :public Object
{
public:	
	SceneObject(const string& aName = "SceneObject");
	SceneObject(SceneObject& aSObject) = delete;
	virtual ~SceneObject();	

public:
	//组件相关设置

	/**
	 * @brief C++内置关键字string(typeid(*this).name())可以在运行时获得类型的字符串名称
	 * 在添加组件的时候，将字符串和组件一起注册到map中
	 */
	template<typename T>
	T* AddComponent(Component* aComp)
	{
		if (!aComp) return nullptr;
		mComponents.insert(pair<string, Component*>(aComp->GetStaticClassName(), aComp));
		return dynamic_cast<T*>(aComp);
	}

	/**
	 * @brief 通过输入的类型的typeid查询map以获得对应组件的引用
	 */
	template<typename T>
	T* GetComponent()
	{
		auto comp_iter = mComponents.find(string(typeid(T).name()));
		if (comp_iter == mComponents.end()) return nullptr;
		return dynamic_cast<T*>((*comp_iter).second);
	}
	
	/** @brief 如果有多个相同组件怎么办呢？*/
	template<typename T>
	int RemoveComponent(const string& aType)
	{
		auto comp_iter = mComponents.find(string(typeid(T).name()));
		if (comp_iter == mComponents.end()) return 0;
		else
		{
			auto comp = dynamic_cast<T*>((*comp_iter).second);
			delete comp; comp = 0;
			mComponents.erase(comp_iter);
		}
		return 1;
	}

	/** @breif 获取当前组件数量*/
	inline int GetComponentNum() const {return (int)mComponents.size();}

	/** @breif 获取当前组件列表*/
	vector<string> GetComponentList()const;

	Component* GetComponent(const string& aCompType);

	vector<pair<string, string>> GetComponentInfo() const;
public:
	//几何属性设置
	//这里没有包含Transform组件的头，不能inline
	 
	Vec3 GetObjectPosition() const;
	Vec3 GetObjectSize() const;
	Vec3 GetObjectRotation()const;

	void SetObejctPosition(const Vec3& aPosition);
	void SetObejctPosition(float x, float y, float z);

	void SetObjectSize(const Vec3& aSize);
	void SetObjectSize(float x, float y, float z);

	void SetObjectRotation(const Vec3& aRotation);
	void SetObjectRotation(float x, float y, float z);

	void AddObjectMovement(const Vec3& aDelta);
	void AddObjectRotation(const Vec3& aDelta);
	
	//void AddObjectMovement(float x, float y, float z);

public:

	virtual void Start();
	virtual void Update();

public:
	//渲染属性设置

	/** 游戏物体是否可见（是否渲染）*/
	bool bIsVisible;
	/** 游戏物体的销毁标记，场景会在特定时间销毁被标记的游戏物体（每隔几秒）*/
	bool bIsDestroy;

	inline bool GetIsVisible() const { return bIsVisible; }

	inline bool GetIsDestroy() const { return bIsDestroy; }
	
	inline void SetVisible(bool aBool) { bIsVisible = aBool; }

	inline void SetDestroy(bool aBool) { bIsDestroy = aBool; }
		 
protected:
	//使用一个map来存怎么样
	Transform* mTransformComp;	

protected:
	map<string, Component*> mComponents;
};

