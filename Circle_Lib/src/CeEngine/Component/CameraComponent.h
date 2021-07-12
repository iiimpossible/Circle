#pragma once
#include "Component.h"



/**
 * @breif �������������κ������ṩ���������
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
	//�����ǣ�X��
	void LookUp(float aPitch);
	//ƫ����Y��
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

 