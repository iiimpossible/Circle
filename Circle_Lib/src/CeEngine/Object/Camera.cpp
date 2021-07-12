#include "Camera.h"
#include <Celib_x64/src/CeEngine/Manager/InputManager.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
#include <Celib_x64/src/CeCommon/OpenGL/CeWindow.h>
#include <Celib_x64/src/CeEngine/Engine.h>
#include <Celib_x64/src/CeCommon/Tool/Time.h>


Camera::Camera(string aName) :
	GameObject(aName),
	mFront(Vec3(0.0f, 0.0f, 1.0f)),
	MovementSpeed(2.5f),
	MouseSensitivity(0.1f),
	Zoom(45.0f)
{
	mRight = Vec3(-1.0f,0.0f,0.0f);
	ProjectionMode = EProjectionMode::Orthographic;
	Yaw = 0.0f;
	Pitch = 0.0f;

	//����Ҽ��Ĳ���
	InputManager::CreateAction("RightButtonPress", EKey::EK_MouseButton_Right);
	InputManager::BindAction("RightButtonPress",this,EKeyState::EKS_Pressed,&Camera::OnRightButtonPressed);
	InputManager::CreateAction("RightButtonRelease", EKey::EK_MouseButton_Right);
	InputManager::BindAction("RightButtonRelease", this, EKeyState::EKS_Released, &Camera::OnRightButtonPressed);

	//����м�����Ұ����
	InputManager::CreateAxis("ScrollFov", EKey::EK_MouseMove_Middle, 0.5f);
	InputManager::BindAxis("ScrollFov", this, &Camera::ProcessMouseScroll);

	//����ӽǱ任
	InputManager::CreateAxis("MoveForward",EKey::EK_W, 0.1f);
	InputManager::CreateAxis("MoveForward", EKey::EK_S, -0.1f);
	InputManager::CreateAxis("MoveRight", EKey::EK_D, 0.1f);
	InputManager::CreateAxis("MoveRight", EKey::EK_A, -0.1f);
	////����ƶ��¼��ķ���ֵҪ����0.
	InputManager::CreateAxis("LookUp", EKey::EK_MouseMove_Y, 0.1f);
	InputManager::CreateAxis("LookRight", EKey::EK_MouseMove_X, 0.1f);

	InputManager::BindAxis("MoveForward",this,&Camera::MoveForward);
	InputManager::BindAxis("MoveRight", this, &Camera::MoveRight);

	InputManager::BindAxis("LookUp", this, &Camera::LookUp);
	InputManager::BindAxis("LookRight", this, &Camera::LookRight);

	SetObejctPosition(Vec3(0.0f,100.0f,0.0f));

	isRightButtonPressed = false;
}
Camera::~Camera(){}

void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

Vec3 Camera::GetFrontVector()
{
	return mFront;
}

float Camera::GetFOVSize()
{
	return Zoom;
}

//TODO:���ڴ�С�ı���һ���ص����������������ִ���ǣ��ص����� ����ͶӰ����
//����ͶӰ����û��Ҫ���Ǹ���
Mat4& Camera::GetProjectionMatrix()
{
	mPerspecMat = CePerspective(Zoom, CeGraphWindow::GetWindowSize().x / CeGraphWindow::GetWindowSize().y, 0.1f, 1000.0f);
	return mPerspecMat;	
}

Mat4& Camera::GetViewMatrix()
{	
	mViewMat = CeLoolAt(GetObjectPosition(), GetObjectPosition() + mFront, Vec3(0.0f, 1.0f, 0.0f));
	return mViewMat;
}
glm::vec3 front;
void Camera::OnRightButtonPressed(float aTag)
{
	isRightButtonPressed = !isRightButtonPressed;	
}

void Camera::MoveForward(float aValue)
{	
	this->AddObjectMovement( mFront * aValue * 10000.0f * CeTimer::GetDeltaTime());
	//DebugLogManager::DebugOutput("Camera", EDebugTag::EDT_Log, { "DaltaTime is: ",  to_string(CeTimer::GetDeltaTime()) });
	//DebugLogManager::DebugOutput("Camera", EDebugTag::EDT_Log, { "Value is: ",  to_string(aValue) });
}

void Camera::MoveRight(float aValue)
{	 
	this->AddObjectMovement(mRight * aValue * 500.0f *  CeTimer::GetDeltaTime());
	//DebugLogManager::DebugOutput("Camera", EDebugTag::EDT_Log, { "DaltaTime is: ",  to_string(CeTimer::GetDeltaTime()) });
	//DebugLogManager::DebugOutput("Camera", EDebugTag::EDT_Log, { "Value is: ",  to_string(aValue) });
}

void Camera::LookUp(float aPitch)
{		
	if (isRightButtonPressed)
	{		
		if (GetObjectRotation().x + aPitch > 89.0f)
		{
			aPitch = 89.0f - GetObjectRotation().x;
		}			
		if (GetObjectRotation().x + aPitch < -89.0f)
		{
			aPitch = -89.0f - GetObjectRotation().x;
		}		
		Mat4 rot;
		Vec4 temp_front(mFront.x, mFront.y, mFront.z, 1.0f);	
		//��ǰ��������ת��Ŀ��Ƕȣ���������ת�����������ת�Ƕȣ�������ת�ᣬ��ǰ������������������������������
		rot = CeRotate(rot, CeRadians(aPitch),CeNormalize(CeCross(mFront,Vec3(0.0f,-1.0f,0.0f))));
		temp_front = rot * temp_front;
		mFront = CeNormalize(temp_front);
		mRight = CeNormalize(CeCross(mFront, Vec3(0.0f, 1.0f, 0.0f)));	
		AddObjectRotation(Vec3(aPitch,0.0f,0.0f));					
	}
}

void Camera::LookRight(float aYaw)
{
	if (isRightButtonPressed)
	{		 
		Mat4 rot;
		Vec4 temp_front(mFront.x, mFront.y, mFront.z, 1.0f);
		/*Vec3 first = CeCross(mFront, Vec3(0.0f, -1.0f, 0.0f));//����ǰ���Y��Ĳ��
		Vec3 second = CeCross(mFront, first);*///����first��ǰ��Ĳ�ˣ������ǰ��F~Y��ƽ�����F��ֱ��ƫ����ת��
		rot = CeRotate(rot, CeRadians(aYaw), Vec3(0.0f, 1.0f, 0.0f));

		//DebugLogManager::DebugOutputMat4("Camera", rot);
		temp_front = rot * temp_front;
		mFront = CeNormalize(temp_front);
		mRight = CeNormalize(CeCross(mFront, Vec3(0.0f, 1.0f, 0.0f)));
		//DebugLogManager::DebugOutputVec3("Camera", Vec3(temp_front));
	}
}


