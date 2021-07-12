#pragma once
#include "GameObject.h"
#include <Celib_x64/src/CeCommon/Common.h>
//#include <Celib_x64/src/CeEngine/Object/ObjectFactory.h>
enum class EProjectionMode :uint8_t 
{
	Perspective = 0,
	Orthographic = 1
};

//// Default camera values
//const float YAW = 0.0f;
//const float PITCH = 0.0f;
//const float SPEED = 2.5f;
//const float SENSITIVITY = 0.1f;
//const float ZOOM = 45.0f;

/**
*@brief ������࣬�̳���GameObject.
*@details �����ƶ����༭ʱ������Ⱦ���ṩ͸�Ӿ���
*/ 
class Camera :public GameObject
{
public:
	Camera(string aName = "Camera");
	~Camera();	
public:	
	//�ӿ�

	Vec3 GetFrontVector();

	float GetFOVSize();
	 
	Mat4& GetProjectionMatrix();

	Mat4& GetViewMatrix(); 	

private:
	//�ص�����

	//����ӳ�����ӳ��
	void OnRightButtonPressed(float aTag);

	void MoveForward(float aValue);

	void MoveRight(float aValue);
	//�����ǣ�X��
	void LookUp(float aPitch);
	//ƫ����Y��
	void LookRight(float aYaw);

	void ProcessMouseScroll(float yoffset);
private:

	//ͶӰģʽ
	EProjectionMode ProjectionMode;
	//͸��ͶӰʱ�Ľ�ƽ��
	float Near;
	//͸��ͶӰʱ��Զƽ��
	float Far;
	//
	float FieldOfView;

	Vec3 mFront;
	Vec3 mUp;
	Vec3 mRight;

	Mat4 mPerspecMat;
	Mat4 mOrthoMat;
	Mat4 mViewMat;

	float Yaw;
	float Pitch;
	float MovementSpeed = 0.0f;
	float MouseSensitivity;
	float Zoom;

	bool isRightButtonPressed;
};

