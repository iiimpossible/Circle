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
*@brief 摄像机类，继承自GameObject.
*@details 具有移动、编辑时网格渲染、提供透视矩阵
*/ 
class Camera :public GameObject
{
public:
	Camera(string aName = "Camera");
	~Camera();	
public:	
	//接口

	Vec3 GetFrontVector();

	float GetFOVSize();
	 
	Mat4& GetProjectionMatrix();

	Mat4& GetViewMatrix(); 	

private:
	//回调函数

	//按键映射和轴映射
	void OnRightButtonPressed(float aTag);

	void MoveForward(float aValue);

	void MoveRight(float aValue);
	//俯仰角，X轴
	void LookUp(float aPitch);
	//偏航角Y轴
	void LookRight(float aYaw);

	void ProcessMouseScroll(float yoffset);
private:

	//投影模式
	EProjectionMode ProjectionMode;
	//透视投影时的近平面
	float Near;
	//透视投影时的远平面
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

