#include "InputManager.h"
//#include "../../CCCommon/OpenGL/CeGraphWindow.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Celib_x64/src/CeCommon/OpenGL/CeWindow.h>
#include <Celib_x64/src/CeEngine/Engine.h>

#pragma region Key_Press
Delegate<void, float> InputManager::OnKeyPressed_A;
Delegate<void, float> InputManager::OnKeyPressed_B;
Delegate<void, float> InputManager::OnKeyPressed_C;
Delegate<void, float> InputManager::OnKeyPressed_D;
Delegate<void, float> InputManager::OnKeyPressed_E;
Delegate<void, float> InputManager::OnKeyPressed_F;
Delegate<void, float> InputManager::OnKeyPressed_G;
Delegate<void, float> InputManager::OnKeyPressed_H;
Delegate<void, float> InputManager::OnKeyPressed_I;
Delegate<void, float> InputManager::OnKeyPressed_J;
Delegate<void, float> InputManager::OnKeyPressed_K;
Delegate<void, float> InputManager::OnKeyPressed_L;
Delegate<void, float> InputManager::OnKeyPressed_M;
Delegate<void, float> InputManager::OnKeyPressed_N;
Delegate<void, float> InputManager::OnKeyPressed_O;
Delegate<void, float> InputManager::OnKeyPressed_P;
Delegate<void, float> InputManager::OnKeyPressed_Q;
Delegate<void, float> InputManager::OnKeyPressed_R;
Delegate<void, float> InputManager::OnKeyPressed_S;
Delegate<void, float> InputManager::OnKeyPressed_T;
Delegate<void, float> InputManager::OnKeyPressed_U;
Delegate<void, float> InputManager::OnKeyPressed_V;
Delegate<void, float> InputManager::OnKeyPressed_W;
Delegate<void, float> InputManager::OnKeyPressed_X;
Delegate<void, float> InputManager::OnKeyPressed_Y;
Delegate<void, float> InputManager::OnKeyPressed_Z;
#pragma endregion	

#pragma region Key_Release
Delegate<void, float> InputManager::OnKeyReleased_A;
Delegate<void, float> InputManager::OnKeyReleased_B;
Delegate<void, float> InputManager::OnKeyReleased_C;
Delegate<void, float> InputManager::OnKeyReleased_D;
Delegate<void, float> InputManager::OnKeyReleased_E;
Delegate<void, float> InputManager::OnKeyReleased_F;
Delegate<void, float> InputManager::OnKeyReleased_G;
Delegate<void, float> InputManager::OnKeyReleased_H;
Delegate<void, float> InputManager::OnKeyReleased_I;
Delegate<void, float> InputManager::OnKeyReleased_J;
Delegate<void, float> InputManager::OnKeyReleased_K;
Delegate<void, float> InputManager::OnKeyReleased_L;
Delegate<void, float> InputManager::OnKeyReleased_M;
Delegate<void, float> InputManager::OnKeyReleased_N;
Delegate<void, float> InputManager::OnKeyReleased_O;
Delegate<void, float> InputManager::OnKeyReleased_P;
Delegate<void, float> InputManager::OnKeyReleased_Q;
Delegate<void, float> InputManager::OnKeyReleased_R;
Delegate<void, float> InputManager::OnKeyReleased_S;
Delegate<void, float> InputManager::OnKeyReleased_T;
Delegate<void, float> InputManager::OnKeyReleased_U;
Delegate<void, float> InputManager::OnKeyReleased_V;
Delegate<void, float> InputManager::OnKeyReleased_W;
Delegate<void, float> InputManager::OnKeyReleased_X;
Delegate<void, float> InputManager::OnKeyReleased_Y;
Delegate<void, float> InputManager::OnKeyReleased_Z;
#pragma endregion
 
#pragma region KeyRepeat
Delegate<void, float> InputManager::OnKeyRepeat_A;
Delegate<void, float> InputManager::OnKeyRepeat_B;
Delegate<void, float> InputManager::OnKeyRepeat_C;
Delegate<void, float> InputManager::OnKeyRepeat_D;
Delegate<void, float> InputManager::OnKeyRepeat_E;
Delegate<void, float> InputManager::OnKeyRepeat_F;
Delegate<void, float> InputManager::OnKeyRepeat_G;
Delegate<void, float> InputManager::OnKeyRepeat_H;
Delegate<void, float> InputManager::OnKeyRepeat_I;
Delegate<void, float> InputManager::OnKeyRepeat_J;
Delegate<void, float> InputManager::OnKeyRepeat_K;
Delegate<void, float> InputManager::OnKeyRepeat_L;
Delegate<void, float> InputManager::OnKeyRepeat_M;
Delegate<void, float> InputManager::OnKeyRepeat_N;
Delegate<void, float> InputManager::OnKeyRepeat_O;
Delegate<void, float> InputManager::OnKeyRepeat_P;
Delegate<void, float> InputManager::OnKeyRepeat_Q;
Delegate<void, float> InputManager::OnKeyRepeat_R;
Delegate<void, float> InputManager::OnKeyRepeat_S;
Delegate<void, float> InputManager::OnKeyRepeat_T;
Delegate<void, float> InputManager::OnKeyRepeat_U;
Delegate<void, float> InputManager::OnKeyRepeat_V;
Delegate<void, float> InputManager::OnKeyRepeat_W;
Delegate<void, float> InputManager::OnKeyRepeat_X;
Delegate<void, float> InputManager::OnKeyRepeat_Y;
Delegate<void, float> InputManager::OnKeyRepeat_Z;
#pragma endregion

Delegate<void, float> InputManager::OnMouseMove_X;
Delegate<void, float> InputManager::OnMouseMove_Y;
Delegate<void, float> InputManager::OnMouseMove_Middle;
Delegate<void, float> InputManager::OnMouseButtonPressed_Left;
Delegate<void, float> InputManager::OnMouseButtonPressed_Right;
Delegate<void, float> InputManager::OnMouseButtonPressed_Middle;
Delegate<void, float> InputManager::OnMouseButtonReleased_Left;
Delegate<void, float> InputManager::OnMouseButtonReleased_Right;
Delegate<void, float> InputManager::OnMouseButtonReleased_Middle;

EKeyState InputManager::KeyState_A;
EKeyState InputManager::KeyState_D;
EKeyState InputManager::KeyState_W;
EKeyState InputManager::KeyState_S;
EKeyState InputManager::KeyState_Space;
EKeyState InputManager::KeyState_MouseLeft;
EKeyState InputManager::KeyState_MouseRight;
EKeyState InputManager::KeyState_MouseMiddle;

unordered_map<string, vector<EKey>> InputManager::mKeyAxisMap;
unordered_map<string, EKey> InputManager::mKeyActionMap; 
unordered_map<EKey, float> InputManager::mKeyAxisValue;

bool InputManager::bHasCreated = false;


InputManager::InputManager()
{
	//能够同时按键
	//glfwSetInputMode(CeGraphWindow::GetGLFWWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
	if (Engine::mWindow)
	{
		GLFWwindow* window = Engine::mWindow->GetGLFWWindow();
		glfwSetKeyCallback(window, &KeyCallBack);
		glfwSetMouseButtonCallback(window,&MouseButtonCallback);
		//获得鼠标回调函数
		glfwSetCursorPosCallback(window, &CursorCallback);

		glfwSetScrollCallback(window, &MouseMiddlwScrollCallback);
		//glfwGetMouseButton(window,);
	}	
}

 

InputManager::~InputManager()
{
}

InputManager* InputManager::CreateInputManager()
{
	if (!bHasCreated)
	{
		bHasCreated = true;
		return new InputManager();
	}
	return nullptr;	
} 

void InputManager::CreateAction(string action_name, EKey key)
{
	if (mKeyActionMap.find(action_name) == mKeyActionMap.end())
	{
		mKeyActionMap.insert(std::pair<string, EKey>(action_name, key));
	}	 
}

/**
* @brief 创建一个轴映射
* 如果没有创建，那么就创建一个轴映射，将该按键加入到轴映射中去，且记录该轴的按键的返回值
* 如果创建了，就是修改该按键的返回值，且将该按键加入到轴映射中去
* 
*/
void InputManager::CreateAxis(string axis_name, EKey key, float return_value)
{
	//存储按键的返回值
	mKeyAxisValue.insert(std::pair<EKey, float>(key, return_value));
	//判断是否已经创建了轴
	if (mKeyAxisMap.find(axis_name) == mKeyAxisMap.end())
	{		 
		mKeyAxisMap.insert(std::pair<string, vector<EKey>>(axis_name, { key }));
	}
	else
	{
		//按键重复检测
		for (auto val : mKeyAxisMap[axis_name])
		{
			if (val == key)
			{
				return;
			}
		}		 
		//将按键加入到已有的轴映射上去
		mKeyAxisMap[axis_name].push_back(key);
	}
	
}

void InputManager::Update()
{
	 
}

 
 

void InputManager::FindAxis(string & axis_name)
{
}

void InputManager::FindAction(string & action_name)
{
}

void InputManager::RemoveAxis(string & axis_name)
{
}

void InputManager::RemoveAction(string & action_name)
{
}
/**
*  @brief 按键回调函数
*  @param[in] window 接收这个事件的OpenGL窗口
*  @param[in] key 被按下或则释放的按键
*  @param[in] scancode 系统指定的特定按键代码
*  @param[in] action 按键状态，按下、释放或者重复
*  releases may add more actions.
*  @param[in] mods Bit field describing which[modifier keys](@ref mods) were
*  held down.
*/
void InputManager::KeyCallBack(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	 
	switch (key)
	{
	case EK_A:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_A(EKS_Pressed);
		}
		else if(action == EKeyState::EKS_Released)
		{
			OnKeyReleased_A(EKS_Released);
		}	
		else if (action == EKeyState::EKS_Repeat)
		{
			OnKeyRepeat_A(mKeyAxisValue[EK_A]);//这个输入值是用户类自定义的
		}		 
		break;
	case EK_B:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_B(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_B(EKS_Released);
		}
		break;
	case EK_C:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_C(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_C(EKS_Released);
		}
		break;
	case EK_D:
		if (action == EKeyState::EKS_Pressed)
		{
			//DebugLog("D EKS_Pressed!");
			OnKeyPressed_D(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			//DebugLog("D EKS_Released!");
			OnKeyReleased_D(EKS_Released);
		}
		else if (action == EKeyState::EKS_Repeat)
		{
			//DebugLog("D EKS_Repeat!");
			OnKeyRepeat_D(mKeyAxisValue[EK_D]);
		}
		break;
	case EK_E:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_E(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_E(EKS_Released);
		}
		break;
	case EK_F:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_F(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_F(EKS_Released);
		}
		break;
	case EK_G:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_G(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_G(EKS_Released);
		}
		break;
	case EK_H:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_H(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_H(EKS_Released);
		}
		break;
	case EK_I:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_I(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_I(EKS_Released);
		}
		break;
	case EK_J:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_J(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_J(EKS_Released);
		}
		break;
	case EK_K:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_K(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_K(EKS_Released);
		}
		break;
	case EK_L:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_L(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_L(EKS_Released);
		}
		break;
	case EK_M:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_M(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_M(EKS_Released);
		}
		break;
	case EK_N:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_N(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_N(EKS_Released);
		}
		break;
	case EK_O:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_O(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_O(EKS_Released);
		}
		break;
	case EK_P:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_P(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_P(EKS_Released);
		}
		break; 
	case EK_Q:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_Q(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_Q(EKS_Released);
		}
		break;
	case EK_R:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_R(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_R(EKS_Released);
		}
		break;
	case EK_S:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_S(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_S(EKS_Released);
		}
		else if (action == EKeyState::EKS_Repeat)
		{
			OnKeyRepeat_S(mKeyAxisValue[EK_S]);
		}
		break;
	case EK_T:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_T(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_T(EKS_Released);
		}
		break;
	case EK_U:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_U(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_U(EKS_Released);
		}
		break;
	case EK_V:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_V(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_V(EKS_Released);
		}
		break;
	case EK_W:
		if (action == EKeyState::EKS_Pressed)
		{
			//DebugLog("W!");
			OnKeyPressed_W(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_W(EKS_Released);
		}
		else if (action == EKeyState::EKS_Repeat)
		{
			OnKeyRepeat_W(mKeyAxisValue[EK_W]);
		}
		break;
	case EK_X:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_X(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_X(EKS_Released);
		}
		break;
	case EK_Y:
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_Y(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_Y(EKS_Released);
		}
		break;
	case EK_Z:
		DebugLogManager::DebugOutput("InputManager", EDebugTag::EDT_Log, { "Right mouse button pressed." });
		if (action == EKeyState::EKS_Pressed)
		{
			OnKeyPressed_Z(EKS_Pressed);
		}
		else if (action == EKeyState::EKS_Released)
		{
			OnKeyReleased_Z(EKS_Released);
		}
		break;
	default:
		break;
	}
		
}

void InputManager::GetKey(GLFWwindow * window)
{	 
	KeyState_A = (EKeyState)glfwGetKey(window, GLFW_KEY_A);
	KeyState_D = (EKeyState)glfwGetKey(window, GLFW_KEY_D);
	KeyState_S = (EKeyState)glfwGetKey(window, GLFW_KEY_S);
	KeyState_W = (EKeyState)glfwGetKey(window, GLFW_KEY_W);
	KeyState_Space = (EKeyState)glfwGetKey(window, GLFW_KEY_SPACE);
	
	KeyState_MouseLeft = (EKeyState)glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT);
	KeyState_MouseRight = (EKeyState)glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT);
	KeyState_MouseMiddle = (EKeyState)glfwGetKey(window, GLFW_MOUSE_BUTTON_MIDDLE);	
}
static float LastXPos = 400.0f;
static float LastYPos = 300.0f;
static float delta = 0.0f;
#include <algorithm>
using namespace std;
void InputManager::CursorCallback(GLFWwindow * aWindow, double aXPos, double aYPos)
{	
	if (mKeyAxisValue.empty()) return;
	float MouseXValue =  mKeyAxisValue[EK_MouseMove_X];
	float MouseYValue =  mKeyAxisValue[EK_MouseMove_Y];
	//如果当前指针的X值大于上一帧的指针X值，意味着鼠标向右移动

	delta = aYPos - LastYPos;
	OnMouseMove_Y(delta * MouseYValue);

	delta = aXPos - LastXPos;
	OnMouseMove_X(delta * MouseXValue);
	/*DebugLogManager::DebugOutput("InputManager", EDebugTag::EDT_Log, 
		{ "Delta is: ", to_string(MouseXValue), ",", to_string(MouseYValue) });*/
	
	LastXPos = aXPos;
	LastYPos = aYPos;
}

void InputManager::MouseButtonCallback(GLFWwindow* aWindow, int aButton, int aAction, int aMods)
{
	switch (aButton)
	{
	case EKey::EK_MouseButton_Left:
		if (aAction == EKeyState::EKS_Pressed)
		{
			OnMouseButtonPressed_Left(EKS_Pressed);
		}
		else if (aAction == EKeyState::EKS_Released)
		{
			OnMouseButtonReleased_Left(EKS_Released);
		}
		break;
	case EKey::EK_MouseButton_Right:
		if (aAction == EKeyState::EKS_Pressed)
		{
			OnMouseButtonPressed_Right(EKS_Pressed);
		}
		else if (aAction == EKeyState::EKS_Released)
		{
			OnMouseButtonReleased_Right(EKS_Released);
		}
		break;
	case EKey::EK_MouseButton_Middle:
		if (aAction == EKeyState::EKS_Pressed)
		{
			OnMouseButtonPressed_Middle(EKS_Pressed);
		}
		else if (aAction == EKeyState::EKS_Released)
		{
			OnMouseButtonReleased_Middle(EKS_Released);
		}
		break;
	default:
		break;
	}

}

void InputManager::MouseMiddlwScrollCallback(GLFWwindow * aWindow, double aXOffset, double aYOffset)
{
	OnMouseMove_Middle(aYOffset * mKeyAxisValue[EK_MouseMove_Middle]);
}
 