#pragma once
#include <Celib_x64/src/CeCommon/Common.h>
#include "Component.h"
using namespace CircleEngine;

/**
*@brief
*@details
��������ĳ��ȵ�λ����2D����£���׼�豸����ϵ������ͶӰ����ѹ����һ�����أ�
����ͬ���豸�����ܶȲ�һ����������ʾ�Ĵ�С��һ��
��100��������Ϊ1����ô����
3D ����£��豸��׼����û�б�ѹ������ˣ����Ƕ��پͶ���
*/
class Transform:public Component
{	
public:

	Transform(GameObject* aOwner = nullptr, const string& aName = "Transform");
	~Transform();

	Transform(Transform& transform) = delete;
	Transform& operator=(Transform& transform) = delete;
public:
	
	inline void AddObjectMovement(const Vec3& aDelta) { mPosition += aDelta; }

	inline void AddObjectRotation(const Vec3& aDelta) { mRotation += aDelta; }
public:

	inline Vec3 GetPosition() const { return mPosition; }

	inline Vec3 GetRotation()const { return mRotation; }

	inline Vec3 GetSize()const { return mSize; }
public:

	inline void SetPosition(const Vec3& aPos) { mPosition = aPos; }

	inline void SetRotation(const Vec3& aRot) { mRotation = aRot; }

	inline void SetSize(const Vec3& aSize) { mSize = aSize; }

public:

	virtual void SetCommand(const string& aCmd)override;

	virtual string GetCommand() override;
public:

	virtual void Start()override;
	virtual void Update() override;

private:

	

private:

	Vec3 mPosition;
	Vec3 mSize;
	Vec3 mRotation;
};

