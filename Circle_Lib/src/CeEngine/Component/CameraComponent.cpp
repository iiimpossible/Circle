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
	if (bIsFirstLook)//û�취��Ŀǰ����������취����ֹ��Ϸ��������ʱŷ���ǲ��Ե��²�����ȷ��ת�ӽ�
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
		//��ǰ��������ת��Ŀ��Ƕȣ���������ת�����������ת�Ƕȣ�������ת�ᣬ��ǰ������������������������������
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
	/*Vec3 first = CeCross(mFront, Vec3(0.0f, -1.0f, 0.0f));//����ǰ���Y��Ĳ��
	Vec3 second = CeCross(mFront, first);*///����first��ǰ��Ĳ�ˣ������ǰ��F~Y��ƽ�����F��ֱ��ƫ����ת��
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
