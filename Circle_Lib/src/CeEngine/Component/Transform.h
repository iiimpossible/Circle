#pragma once
#include <Celib_x64/src/CeCommon/Common.h>
#include "Component.h"
using namespace CircleEngine;

/**
*@brief
*@details
关于世界的长度单位，在2D情况下，标准设备坐标系被正射投影矩阵压缩到一个像素，
而不同的设备像素密度不一样，导致显示的大小不一样
将100个像素作为1米怎么样？
3D 情况下，设备标准坐标没有被压缩，因此，该是多少就多少
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

