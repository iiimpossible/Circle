#pragma once
#include "Component.h"



/**
 * @breif 摄像机组件，给任何物体提供摄像机属性
 */
class CameraComponent: public Component
{
public:
	CameraComponent(GameObject* aOwner = nullptr, const string& aName = "CameraComponent");
	~CameraComponent();
public:

	inline Vec3 GetFrontVector() { return mFront; }

	inline float GetFOVSize() { return mFOV; }

	inline float GetZoom() { return mZoom; }

	inline void SetZoom(float aZoom) { mZoom = aZoom; }

	inline Vec3 GetRightVector() { return mRight; }



public:

	Mat4& GetProjectionMatrix();

	Mat4& GetViewMatrix();


	void MoveForward(float aValue);

	void MoveRight(float aValue);
	//俯仰角，X轴
	void LookUp(float aPitch);
	//偏航角Y轴
	void LookRight(float aYaw);

	void ProcessMouseScroll(float yoffset);


public:

	Vec3 GetObjectPosition();





public:

	virtual void Start() override;

	virtual void Update()override;

	virtual void SetCommand(const string& aCmd) override;

private:	

	Mat4 mProjMat;	 
	Mat4 mViewMat;

	Vec3 mFront;
	Vec3 mUp;
	Vec3 mRight;

	float mFOV;//field of view
	float mZoom;

	bool bIsFirstLook;
};

 