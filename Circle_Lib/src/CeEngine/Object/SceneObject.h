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
 * @brief ��ļ򵥸���
 * @details �����ϸ����
 */
class SceneObject :public Object
{
public:	
	SceneObject(const string& aName = "SceneObject");
	SceneObject(SceneObject& aSObject) = delete;
	virtual ~SceneObject();	

public:
	//����������

	/**
	 * @brief C++���ùؼ���string(typeid(*this).name())����������ʱ������͵��ַ�������
	 * ����������ʱ�򣬽��ַ��������һ��ע�ᵽmap��
	 */
	template<typename T>
	T* AddComponent(Component* aComp)
	{
		if (!aComp) return nullptr;
		mComponents.insert(pair<string, Component*>(aComp->GetStaticClassName(), aComp));
		return dynamic_cast<T*>(aComp);
	}

	/**
	 * @brief ͨ����������͵�typeid��ѯmap�Ի�ö�Ӧ���������
	 */
	template<typename T>
	T* GetComponent()
	{
		auto comp_iter = mComponents.find(string(typeid(T).name()));
		if (comp_iter == mComponents.end()) return nullptr;
		return dynamic_cast<T*>((*comp_iter).second);
	}
	
	/** @brief ����ж����ͬ�����ô���أ�*/
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

	/** @breif ��ȡ��ǰ�������*/
	inline int GetComponentNum() const {return (int)mComponents.size();}

	/** @breif ��ȡ��ǰ����б�*/
	vector<string> GetComponentList()const;

	Component* GetComponent(const string& aCompType);

	vector<pair<string, string>> GetComponentInfo() const;
public:
	//������������
	//����û�а���Transform�����ͷ������inline
	 
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
	//��Ⱦ��������

	/** ��Ϸ�����Ƿ�ɼ����Ƿ���Ⱦ��*/
	bool bIsVisible;
	/** ��Ϸ��������ٱ�ǣ����������ض�ʱ�����ٱ���ǵ���Ϸ���壨ÿ�����룩*/
	bool bIsDestroy;

	inline bool GetIsVisible() const { return bIsVisible; }

	inline bool GetIsDestroy() const { return bIsDestroy; }
	
	inline void SetVisible(bool aBool) { bIsVisible = aBool; }

	inline void SetDestroy(bool aBool) { bIsDestroy = aBool; }
		 
protected:
	//ʹ��һ��map������ô��
	Transform* mTransformComp;	

protected:
	map<string, Component*> mComponents;
};

