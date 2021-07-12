#pragma once
#include <Celib_x64/src/CeCommon/Common.h>
#include "Component.h"
#include "../Render/Material.h"
#include "../Render/StaticMesh.h"
class GameObject;
/**
* @brief �����ṩ��Ⱦ����Ķ�����Ϣ�͹��ղ���
* @note Ŀǰ�ǻ��ڷ��Ϲ���ģ�͵Ĺ���ϵͳ
* ��Ⱦ����Ҫ��ɫ�������㻺�塢�����Handle

*/
class MeshRenderer:public Component
{
public:
	MeshRenderer(GameObject* aObejct = nullptr);
	~MeshRenderer();
public:
	//Set
	inline void SetStaticMesh(const StaticMesh& aMesh) { mStaticMesh = aMesh; }
	inline void SetMaterial(Material& aMaterial)
	{		
		aMaterial.SetEmissiveColor(mMaterial.GetEmissiveColor());
		mMaterial = aMaterial;
		mCurrentCommand = mMaterial.GetPath();
	}

	void SetMaterial(const string& aPath, const string& aName);
	void SetMaterialInstance(Material& aMat);

	inline void SetIgnore(bool aIgnore) { bIgnore = aIgnore; }
	inline bool GetIgnore() { return bIgnore; }
	
	virtual void SetCommand(const string& aCmd)override;
	
public:
	//Get
	inline StaticMesh& GetStaticMesh() { return mStaticMesh; }
	inline Material& GetMaterail() { return mMaterial; }

public:

	virtual void Start()override;
	virtual void Update()override;

private:	

	Material mMaterial;
	StaticMesh mStaticMesh;

	bool bIgnore;
};

 