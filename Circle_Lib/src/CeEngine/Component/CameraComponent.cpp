#include "CameraComponent.h"
#include <CeCommon/OpenGL/CeWindow.h>
#include <CeEngine/Object/GameObject.h>
#include <CeCommon/Tool/Time.h>
#include <CeEngine/Manager/DebugLogManager.h>

CameraComponent::CameraComponent(GameObject * aOwner, const string & aName):
	Component(aOwner,aName)
{
	mZoom = 45.0f;
	mFront = Vec3(0.0f, 0.0f, 1.0f);
	mRight = Vec3(-1.0f, 0.0f, 0.0f);
	mOwner->SetObjectRotation(Vec3(0.0f));

	bIsFirstLook = true;

}

CameraComponent::~CameraComponent()
{

}

Mat4 & CameraComponent::GetProjectionMatrix()
{
	mProjMat = CePerspective(mZoom, CeGraphWindow::GetWindowSize().x / CeGraphWindow::GetWindowSize().y, 0.1f, 1000.0f);
	return mProjMat;
}

Mat4 & CameraComponent::GetViewMatrix()
{
	if (mOwner)
	{
		mViewMat = CeLoolAt(mOwner->GetObjectPosition(), mOwner->GetObjectPosition() + mFront, Vec3(0.0f, 1.0f, 0.0f));
	}	
	return mViewMat;
}

void CameraComponent::MoveForward(float aValue)
{
	mOwner->AddObjectMovement(mFront * aValue * 1000.0f * CeTimer::GetDeltaTime());
}

void CameraComponent::MoveRight(float aValue)
{
	mOwner->AddObjectMovement(mRight * aValue * 50.0f *  CeTimer::GetDeltaTime());
}

void CameraComponent::LookUp(float aPitch)
{
	if (bIsFirstLook)//没办法，目前就用这个笨办法来防止游戏物体生成时欧拉角不对导致不能正确旋转视角
	{
		mOwner->SetObjectRotation(Vec3(0.0f));
		bIsFirstLook = false;
	}
	if (mOwner)
	{
		//DebugLogManager::DebugOutput("CameraComp", EDebugTag::EDT_Log,
			//{ "Value isL: ",to_string(mOwner->GetObjectRotation().x + aPitch) });
		if (mOwner->GetObjectRotation().x + aPitch > 89.0f)
		{
			aPitch = 89.0f - mOwner->GetObjectRotation().x;			

		}
		if (mOwner->GetObjectRotation().x + aPitch < -89.0f)
		{
			aPitch = -89.0f - mOwner->GetObjectRotation().x;			

		}
		Mat4 rot;
		Vec4 temp_front(mFront.x, mFront.y, mFront.z, 1.0f);
		//将前向向量旋转到目标角度（参数：旋转矩阵输出，旋转角度，计算旋转轴，即前向向量和世界下向量的正交向量）
		rot = CeRotate(rot, CeRadians(aPitch), CeNormalize(CeCross(mFront, Vec3(0.0f, -1.0f, 0.0f))));
		temp_front = rot * temp_front;
		mFront = CeNormalize(temp_front);
		mRight = CeNormalize(CeCross(mFront, Vec3(0.0f, 1.0f, 0.0f)));
		mOwner->AddObjectRotation(Vec3(aPitch, 0.0f, 0.0f));
		//DebugLogManager::DebugOutput("CameraComp", EDebugTag::EDT_Log, {"Look up.", to_string(aPitch)});
	}
	
	 

}

void CameraComponent::LookRight(float aYaw)
{

	Mat4 rot;
	Vec4 temp_front(mFront.x, mFront.y, mFront.z, 1.0f);
	/*Vec3 first = CeCross(mFront, Vec3(0.0f, -1.0f, 0.0f));//计算前向和Y轴的叉乘
	Vec3 second = CeCross(mFront, first);*///计算first和前向的叉乘，获得在前向F~Y轴平面的与F垂直的偏航旋转轴
	rot = CeRotate(rot, CeRadians(aYaw), Vec3(0.0f, 1.0f, 0.0f));

	//DebugLogManager::DebugOutputMat4("Camera", rot);
	temp_front = rot * temp_front;
	mFront = CeNormalize(temp_front);
	mRight = CeNormalize(CeCross(mFront, Vec3(0.0f, 1.0f, 0.0f)));
	//DebugLogManager::DebugOutputVec3("Camera", Vec3(temp_front));
	//DebugLogManager::DebugOutput("CameraComp", EDebugTag::EDT_Log, { "Look right.", to_string(aYaw) });
	 
}

void CameraComponent::ProcessMouseScroll(float yoffset)
{
}

Vec3 CameraComponent::GetObjectPosition()
{

	if (mOwner)
	{
		return mOwner->GetObjectPosition();
	}
	return Vec3();
}

void CameraComponent::Start()
{
}

void CameraComponent::Update()
{
}

void CameraComponent::SetCommand(const string & aCmd)
{
}
