#include "CameraActor.h"
#include <CeEngine/Manager/ResourceManager.h>
#include <CeEngine/Manager/InputManager.h>
#include <CeEngine/Manager/SceneManager.h>
#include <CeEngine/Object/Scene.h>
#include <CeEngine/Component/CameraComponent.h>
#include <CeEngine/Component/MeshRenderer.h>
#include <CeEngine/Render/Material.h>
#include <CeEngine/Render/StaticMesh.h>
#include <CeCommon/Tool/Time.h>
#include <CeEngine/Manager/DebugLogManager.h>
CameraActor::CameraActor(const string & aName) :
	GameObject(aName)
{
	mMeshRender->SetIgnore(true);	 

	mCameraComp = AddComponent<CameraComponent>(new CameraComponent(this));

	mScene = SceneManager::GetCurrentScene();
	mScene->InsertCameraComponent(mCameraComp);

	//鼠标右键的操作
	InputManager::CreateAction("RightButtonPress", EKey::EK_MouseButton_Right);
	InputManager::BindAction("RightButtonPress", this, EKeyState::EKS_Pressed, &CameraActor::OnRightButtonPressed);
	InputManager::CreateAction("RightButtonRelease", EKey::EK_MouseButton_Right);
	InputManager::BindAction("RightButtonRelease", this, EKeyState::EKS_Released, &CameraActor::OnRightButtonRelease);

	//鼠标中键，视野缩放
	InputManager::CreateAxis("ScrollFov", EKey::EK_MouseMove_Middle, 0.5f);
	InputManager::BindAxis("ScrollFov", this, &CameraActor::ProcessMouseScroll);

	//鼠标视角变换
	InputManager::CreateAxis("MoveForward", EKey::EK_W, 0.1f);
	InputManager::CreateAxis("MoveForward", EKey::EK_S, -0.1f);
	InputManager::CreateAxis("MoveRight", EKey::EK_D, 0.1f);
	InputManager::CreateAxis("MoveRight", EKey::EK_A, -0.1f);
	////鼠标移动事件的返回值要大于0.
	InputManager::CreateAxis("LookUp", EKey::EK_MouseMove_Y, 0.1f);
	InputManager::CreateAxis("LookRight", EKey::EK_MouseMove_X, 0.1f);

	InputManager::BindAxis("MoveForward", this, &CameraActor::MoveForward);
	InputManager::BindAxis("MoveRight", this, &CameraActor::MoveRight);

	InputManager::BindAxis("LookUp", this, &CameraActor::LookUp);
	InputManager::BindAxis("LookRight", this, &CameraActor::LookRight);

	SetObejctPosition(Vec3(0.0f, 0.0f, 0.0f));

	isRightButtonPressed = false;



}

CameraActor::~CameraActor()
{
	if (mScene)
	{
		mScene->DeleteCameraComponent(mCameraComp);
	}

}

void CameraActor::Start()
{
}

void CameraActor::Update()
{
}

void CameraActor::OnRightButtonPressed(float aTag)
{
	isRightButtonPressed = true;
}

void CameraActor::OnRightButtonRelease(float aTag)
{
	isRightButtonPressed = false;
}

void CameraActor::MoveForward(float aValue)
{
	if(!bIsDestroy)
		mCameraComp->MoveForward(aValue);	
}

void CameraActor::MoveRight(float aYaw)
{
	//DebugLogManager::DebugOutput("CameraActor", EDebugTag::EDT_Log, { "Look up.", to_string(aYaw) });
	if (!bIsDestroy)
		mCameraComp->MoveRight(aYaw);
}

void CameraActor::LookUp(float aPitch)
{
	//DebugLogManager::DebugOutput("CameraActor", EDebugTag::EDT_Log, { "Look up.", to_string(aPitch) });
	if (isRightButtonPressed && !bIsDestroy)
	{
		mCameraComp->LookUp(aPitch);		
	}
}

void CameraActor::LookRight(float aYaw)
{
	if (isRightButtonPressed && !bIsDestroy)
	{
		mCameraComp->LookRight(aYaw);		
	}
}

void CameraActor::ProcessMouseScroll(float yoffset)
{
	if (!bIsDestroy)
		mCameraComp->ProcessMouseScroll(yoffset);
}
