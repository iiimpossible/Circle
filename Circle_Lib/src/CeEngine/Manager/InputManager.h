#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <Celib_x64/src/CeCommon/Tool/Delegate.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
using std::unordered_map;
using std::string;
using std::vector;
using namespace CircleEngine; 

struct GLFWwindow;

/**
* @brief 按键输入管理
* 轴映射：需要检测所有键盘按键是否按下
*	轴映射是在绑定的按键按下后返回-1 0 1 
*	且轴映射是只要按下不释放就一直输出
* 行为映射：需要检测所有键盘按键是否按下
*      行为映射是区分按下和释放的
*	轴映射需要轴名，按键名，函数回调函数指针，包含回调函数的对象
*
* 实时检测按键状态（仅仅是绑定了的按键）
* 
*/
enum EKeyState :uint8_t
{	
	EKS_Released = 0,
	EKS_Pressed = 1,
	EKS_Repeat = 2
};


//#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1 0
//#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2 1 
//#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3 2
//#define GLFW_RELEASE                0
//*! @brief The key or mouse button was pressed.
// *
// *  The key or mouse button was pressed.
// *
// *  @ingroup input
// */
//#define GLFW_PRESS                  1
// /*! @brief The key was held down until it repeated.
//  *
//  *  The key was held down until it repeated.
//  *
//  *  @ingroup input
//  */
//#define GLFW_REPEAT                 2
enum EKey
{
	EK_A = 65,
	EK_B = 66,
	EK_C = 67,
	EK_D = 68,
	EK_E = 69,
	EK_F = 70,
	EK_G = 71,
	EK_H = 72,
	EK_I = 73,
	EK_J = 74,
	EK_K = 75,
	EK_M = 76,
	EK_L = 77,
	EK_N = 78,
	EK_O = 79,
	EK_P = 80,
	EK_Q = 81,
	EK_R = 82,
	EK_S = 83,
	EK_T = 84,
	EK_U = 85,
	EK_V = 86,
	EK_W = 87,
	EK_X = 88,
	EK_Y = 89,
	EK_Z = 90,

	EK_Space = 32,
	EK_ESC = 256,
	EK_LeftShift = 340,
	EK_Tab = 258,
	EK_LeftCtrl = 341,

	EK_0 = 48,
	EK_1 = 49,
	EK_2 = 50,
	EK_3 = 51,
	EK_4 = 52,
	EK_5 = 53,
	EK_6 = 54,
	EK_7 = 55,
	EK_8 = 56,
	EK_9 = 57,

	EK_MouseButton_Left = 0,
	EK_MouseButton_Right = 1,
	EK_MouseButton_Middle = 3,

	EK_MouseMove_X = 2,
	EK_MouseMove_Y = 3,
	EK_MouseMove_Middle = 4
};
enum class EAxis
{
	EA_X,
	EA_Y,
	EA_Z,

	EA_MouseX,
	EA_MouseY,
	EK_MouceMiddle
};

class InputManager
{
public:

	static InputManager* CreateInputManager();
public:

	/**
	 * @brief 创建一个行为映射，并将按键绑定到行为
	 * @note 一个行为映射只能绑定一个按键
	 */
	static void CreateAction(string action_name, EKey key);
	/**
	 * @brief 创建一个轴映射，绑定将按键绑定到轴
	 * @note 一个轴映射可以同时绑定多个按键
	 */
	static void CreateAxis(string axis_name, EKey key, float return_value = 1.0f);
	static void Update();//尝试在这个里边实现无延迟的轴映射
	static void FindAxis(string& axis_name);
	static void FindAction(string& action_name);
	static void RemoveAxis(string& axis_name);
	static void RemoveAction(string& action_name);	

public:
	//轴映射和按键映射函数模板

	/**注意Func的参数没有意义，因为这个委托实现的原因不能处理参数为void，必须加个参数，即使没有意义  */
	template<typename Object>
	static void BindAction(string action_name, Object* object, EKeyState key_state, void(Object::*Func)(float))
	{
		if (!object || !Func)
		{
			DebugLogManager::DebugOutput("InputManager", EDebugTag::EDT_ERROR, { "Object invalid." });
			return;
		}
		if (key_state == EKeyState::EKS_Pressed)
		{
			if (mKeyActionMap.find(action_name) == mKeyActionMap.end())
			{
				DebugLogManager::DebugOutput("InputManager", EDebugTag::EDT_ERROR, { "Action not exist." });
				return;
			}
			//从Map中查找行为以将回调函数加入到指定事件（多播委托）
			//注意在绑定行为之前，已经使用CreateAction函数创建了特定行为
			switch (mKeyActionMap[action_name])
			{
			case EK_A:
				OnKeyPressed_A += NewDelegate(object, Func);
				break;
			case EK_B:
				OnKeyPressed_B += NewDelegate(object, Func);
				break;
			case EK_C:
				OnKeyPressed_C += NewDelegate(object, Func);
				break;
			case EK_D:
				OnKeyPressed_D += NewDelegate(object, Func);
				break;
			case EK_E:
				OnKeyPressed_E += NewDelegate(object, Func);
				break;
			case EK_F:
				OnKeyPressed_F += NewDelegate(object, Func);
				break;
			case EK_G:
				OnKeyPressed_G += NewDelegate(object, Func);
				break;


			case EK_H:
				OnKeyPressed_H += NewDelegate(object, Func);
				break;
			case EK_I:
				OnKeyPressed_I += NewDelegate(object, Func);
				break;
			case EK_J:
				OnKeyPressed_J += NewDelegate(object, Func);
				break;
			case EK_K:
				OnKeyPressed_K += NewDelegate(object, Func);
				break;
			case EK_L:
				OnKeyPressed_L += NewDelegate(object, Func);
				break;
			case EK_M:
				OnKeyPressed_M += NewDelegate(object, Func);
				break;
			case EK_N:
				OnKeyPressed_N += NewDelegate(object, Func);
				break;


			case EK_O:
				OnKeyPressed_O += NewDelegate(object, Func);
				break;
			case EK_P:
				OnKeyPressed_P += NewDelegate(object, Func);
				break;
			case EK_Q:
				OnKeyPressed_Q += NewDelegate(object, Func);
				break;
			case EK_R:
				OnKeyPressed_R += NewDelegate(object, Func);
				break;
			case EK_S:
				OnKeyPressed_S += NewDelegate(object, Func);
				break;
			case EK_T:
				OnKeyPressed_T += NewDelegate(object, Func);
				break;


			case EK_U:
				OnKeyPressed_U += NewDelegate(object, Func);
				break;
			case EK_V:
				OnKeyPressed_V += NewDelegate(object, Func);
				break;
			case EK_W:
				OnKeyPressed_W += NewDelegate(object, Func);
				break;
			case EK_X:
				OnKeyPressed_X += NewDelegate(object, Func);
				break;
			case EK_Y:
				OnKeyPressed_Y += NewDelegate(object, Func);
				break;
			case EK_Z:
				OnKeyPressed_Z += NewDelegate(object, Func);
				break;

			case EK_MouseButton_Left:
				OnMouseButtonPressed_Left += NewDelegate(object, Func);
				break;
			case EK_MouseButton_Right:
				OnMouseButtonPressed_Right += NewDelegate(object, Func);
				break;
			case EK_MouseButton_Middle:
				OnMouseButtonPressed_Middle += NewDelegate(object, Func);
				break;
			default:
				break;
			}
		}
		else if (key_state == EKeyState::EKS_Released)
		{
			if (mKeyActionMap.find(action_name) == mKeyActionMap.end())
			{
				DebugLogManager::DebugOutput("InputManager", EDebugTag::EDT_ERROR, { "Action not exist." });
				return;
			}
			//从Map中查找行为以将回调函数加入到指定事件（多播委托）
			//注意在绑定行为之前，已经使用CreateAction函数创建了特定行为
			switch (mKeyActionMap[action_name])
			{
			case EK_A:
				OnKeyReleased_A += NewDelegate(object, Func);
				break;
			case EK_B:
				OnKeyReleased_B += NewDelegate(object, Func);
				break;
			case EK_C:
				OnKeyReleased_C += NewDelegate(object, Func);
				break;
			case EK_D:
				OnKeyReleased_D += NewDelegate(object, Func);
				break;
			case EK_E:
				OnKeyReleased_E += NewDelegate(object, Func);
				break;
			case EK_F:
				OnKeyReleased_F += NewDelegate(object, Func);
				break;
			case EK_G:
				OnKeyReleased_G += NewDelegate(object, Func);
				break;


			case EK_H:
				OnKeyReleased_H += NewDelegate(object, Func);
				break;
			case EK_I:
				OnKeyReleased_I += NewDelegate(object, Func);
				break;
			case EK_J:
				OnKeyReleased_J += NewDelegate(object, Func);
				break;
			case EK_K:
				OnKeyReleased_K += NewDelegate(object, Func);
				break;
			case EK_L:
				OnKeyReleased_L += NewDelegate(object, Func);
				break;
			case EK_M:
				OnKeyReleased_M += NewDelegate(object, Func);
				break;
			case EK_N:
				OnKeyReleased_N += NewDelegate(object, Func);
				break;


			case EK_O:
				OnKeyReleased_O += NewDelegate(object, Func);
				break;
			case EK_P:
				OnKeyReleased_P += NewDelegate(object, Func);
				break;
			case EK_Q:
				OnKeyReleased_Q += NewDelegate(object, Func);
				break;
			case EK_R:
				OnKeyReleased_R += NewDelegate(object, Func);
				break;
			case EK_S:
				OnKeyReleased_S += NewDelegate(object, Func);
				break;
			case EK_T:
				OnKeyReleased_T += NewDelegate(object, Func);
				break;


			case EK_U:
				OnKeyReleased_U += NewDelegate(object, Func);
				break;
			case EK_V:
				OnKeyReleased_V += NewDelegate(object, Func);
				break;
			case EK_W:
				OnKeyReleased_W += NewDelegate(object, Func);
				break;
			case EK_X:
				OnKeyReleased_X += NewDelegate(object, Func);
				break;
			case EK_Y:
				OnKeyReleased_Y += NewDelegate(object, Func);
				break;
			case EK_Z:
				OnKeyReleased_Z += NewDelegate(object, Func);
				break;

			case EK_MouseButton_Left:
				OnMouseButtonReleased_Left += NewDelegate(object, Func);
				break;
			case EK_MouseButton_Right:
				OnMouseButtonReleased_Right += NewDelegate(object, Func);
				break;
			case EK_MouseButton_Middle:
				OnMouseButtonReleased_Middle += NewDelegate(object, Func);
				break;
			default:
				break;
			}
		}
	}
	/**
	* @brief 将函数绑定到轴映射。
	* 将创建好的轴上的按键都注册到事件中去，但是如果还要添加按键，需要使用CreateAxis方法添加按键
	* 然后再BindAxis一次。
	* 委托里边有重复判断，不会重复添加函数
	*/
	template<typename Object>
	static void BindAxis(string axis_name, Object* object, void(Object::*Func)(float))
	{
		if (!object || !Func)
		{
			DebugLogManager::DebugOutput("InputManager", EDebugTag::EDT_ERROR, { "Object invalid." });
				return;
		}	 
		if (mKeyAxisMap.find(axis_name) == mKeyAxisMap.end())
		{
			DebugLogManager::DebugOutput("InputManager", EDebugTag::EDT_ERROR, { "Axis not exist. Axis name: ", axis_name });
			return;
		}	 
		//从Map中查找轴以将回调函数加入到指定事件（多播委托）
		//注意在绑定轴之前，已经使用CreateAction函数创建了特定轴
		for (EKey val : mKeyAxisMap[axis_name])
		{
			switch (val)
			{
			case EK_A:
				OnKeyRepeat_A += NewDelegate(object, Func);
				break;	 
			case EK_B:
				OnKeyRepeat_B += NewDelegate(object, Func);
				break;
			case EK_C:
				OnKeyRepeat_C += NewDelegate(object, Func);
				break;
			case EK_D:				
				OnKeyRepeat_D += NewDelegate(object, Func);
				break;
			case EK_E:
				OnKeyRepeat_E += NewDelegate(object, Func);
				break;
			case EK_F:
				OnKeyRepeat_F += NewDelegate(object, Func);
				break;
			case EK_G:
				OnKeyRepeat_G += NewDelegate(object, Func);
				break;


			case EK_H:
				OnKeyRepeat_H += NewDelegate(object, Func);
				break;
			case EK_I:
				OnKeyRepeat_I += NewDelegate(object, Func);
				break;
			case EK_J:
				OnKeyRepeat_J += NewDelegate(object, Func);
				break;
			case EK_K:
				OnKeyRepeat_K += NewDelegate(object, Func);
				break;
			case EK_L:
				OnKeyRepeat_L += NewDelegate(object, Func);
				break;
			case EK_M:
				OnKeyRepeat_M += NewDelegate(object, Func);
				break;
			case EK_N:
				OnKeyRepeat_N += NewDelegate(object, Func);
				break;


			case EK_O:
				OnKeyRepeat_O += NewDelegate(object, Func);
				break;
			case EK_P:
				OnKeyRepeat_P += NewDelegate(object, Func);
				break;
			case EK_Q:
				OnKeyRepeat_Q += NewDelegate(object, Func);
				break;
			case EK_R:
				OnKeyRepeat_R += NewDelegate(object, Func);
				break;
			case EK_S:
				OnKeyRepeat_S += NewDelegate(object, Func);
				break;
			case EK_T:
				OnKeyRepeat_T += NewDelegate(object, Func);
				break;

			case EK_U:
				OnKeyRepeat_U += NewDelegate(object, Func);
				break;
			case EK_V:
				OnKeyRepeat_V += NewDelegate(object, Func);
				break;
			case EK_W:
				OnKeyRepeat_W += NewDelegate(object, Func);
				break;
			case EK_X:
				OnKeyRepeat_X += NewDelegate(object, Func);
				break;
			case EK_Y:
				OnKeyRepeat_Y += NewDelegate(object, Func);
				break;
			case EK_Z:
				OnKeyRepeat_Z += NewDelegate(object, Func);
				break;

			case EK_MouseMove_X:
				OnMouseMove_X += NewDelegate(object, Func);
				break;
			case EK_MouseMove_Y:
				OnMouseMove_Y += NewDelegate(object, Func);
				break;
			case EK_MouseMove_Middle:
				OnMouseMove_Middle += NewDelegate(object, Func);
				break;
			default:
				break;
			}
		} 

	}
	
private:
	InputManager();
	~InputManager();
	InputManager(InputManager& input_manager) = delete;
	InputManager& operator=(InputManager & input_manager) = delete;

	/**
	 * @brief 按键的回调函数，这个函数根据OpenGL获得的按键事件调用绑定的回调函数
	 */
	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void GetKey(GLFWwindow* window);

	/**
	 * @brief 鼠标回调函数，获取指针在屏幕上的位置
	 * 使用这个函数来判断鼠标是否在X轴或者Y轴方向移动
	 * 如果移动那么就激活相应的事件
	 * @detail
	 */
	static void CursorCallback(GLFWwindow* aWindow, double aXPos, double aYPos);
	
	/**
	 * @breif
	 * @code
		void function_name(GLFWwindow* window, int button, int action, int mods)
		@endcode

		@param[in] window The window that received the event.
		@param[in] button The[mouse button](@ref buttons) that was pressed or
		released.
		@param[in] action One of `GLFW_PRESS` or `GLFW_RELEASE`.  Future releases
		may add more actions.
		@param[in] mods Bit field describing which[modifier keys](@ref mods) were
		held down.
	 */	
	static void MouseButtonCallback(GLFWwindow* aWindow,int aButton,int aAction,int aMods);

	static void MouseMiddlwScrollCallback(GLFWwindow* aWindow, double aXOffset,double aYOffset);

	/** 记录每个轴绑定了多少个按键 */
	static unordered_map<string, vector<EKey>> mKeyAxisMap;
	/** 记录每个行为绑定的按键 */
	static unordered_map<string, EKey> mKeyActionMap;
	/** 记录创建轴映射的时候输入的轴的返回值大小 */
	static unordered_map<EKey, float> mKeyAxisValue;

	static bool bHasCreated;

	static Delegate<void, float> OnMouseMove_X;
	static Delegate<void, float> OnMouseMove_Y;
	static Delegate<void, float> OnMouseMove_Middle;//中间滚轮控制
	static Delegate<void, float> OnMouseButtonPressed_Left;
	static Delegate<void, float> OnMouseButtonPressed_Right;
	static Delegate<void, float> OnMouseButtonPressed_Middle;
	static Delegate<void, float> OnMouseButtonReleased_Left;
	static Delegate<void, float> OnMouseButtonReleased_Right;
	static Delegate<void, float> OnMouseButtonReleased_Middle;


	static Delegate<void, float> OnKeyPressed_A;
	static Delegate<void, float> OnKeyPressed_B;
	static Delegate<void, float> OnKeyPressed_C;
	static Delegate<void, float> OnKeyPressed_D;
	static Delegate<void, float> OnKeyPressed_E;
	static Delegate<void, float> OnKeyPressed_F;
	static Delegate<void, float> OnKeyPressed_G;
	static Delegate<void, float> OnKeyPressed_H;
	static Delegate<void, float> OnKeyPressed_I;
	static Delegate<void, float> OnKeyPressed_J;
	static Delegate<void, float> OnKeyPressed_K;
	static Delegate<void, float> OnKeyPressed_L;
	static Delegate<void, float> OnKeyPressed_M;
	static Delegate<void, float> OnKeyPressed_N;
	static Delegate<void, float> OnKeyPressed_O;
	static Delegate<void, float> OnKeyPressed_P;
	static Delegate<void, float> OnKeyPressed_Q;
	static Delegate<void, float> OnKeyPressed_R;
	static Delegate<void, float> OnKeyPressed_S;
	static Delegate<void, float> OnKeyPressed_T;
	static Delegate<void, float> OnKeyPressed_U;
	static Delegate<void, float> OnKeyPressed_V;
	static Delegate<void, float> OnKeyPressed_W;
	static Delegate<void, float> OnKeyPressed_X;
	static Delegate<void, float> OnKeyPressed_Y;
	static Delegate<void, float> OnKeyPressed_Z;

	static Delegate<void, float> OnKeyReleased_A;
	static Delegate<void, float> OnKeyReleased_B;
	static Delegate<void, float> OnKeyReleased_C;
	static Delegate<void, float> OnKeyReleased_D;
	static Delegate<void, float> OnKeyReleased_E;
	static Delegate<void, float> OnKeyReleased_F;
	static Delegate<void, float> OnKeyReleased_G;
	static Delegate<void, float> OnKeyReleased_H;
	static Delegate<void, float> OnKeyReleased_I;
	static Delegate<void, float> OnKeyReleased_J;
	static Delegate<void, float> OnKeyReleased_K;
	static Delegate<void, float> OnKeyReleased_L;
	static Delegate<void, float> OnKeyReleased_M;
	static Delegate<void, float> OnKeyReleased_N;
	static Delegate<void, float> OnKeyReleased_O;
	static Delegate<void, float> OnKeyReleased_P;
	static Delegate<void, float> OnKeyReleased_Q;
	static Delegate<void, float> OnKeyReleased_R;
	static Delegate<void, float> OnKeyReleased_S;
	static Delegate<void, float> OnKeyReleased_T;
	static Delegate<void, float> OnKeyReleased_U;
	static Delegate<void, float> OnKeyReleased_V;
	static Delegate<void, float> OnKeyReleased_W;
	static Delegate<void, float> OnKeyReleased_X;
	static Delegate<void, float> OnKeyReleased_Y;
	static Delegate<void, float> OnKeyReleased_Z;
 
	static Delegate<void, float> OnKeyRepeat_A;
	static Delegate<void, float> OnKeyRepeat_B;
	static Delegate<void, float> OnKeyRepeat_C;
	static Delegate<void, float> OnKeyRepeat_D;
	static Delegate<void, float> OnKeyRepeat_E;
	static Delegate<void, float> OnKeyRepeat_F;
	static Delegate<void, float> OnKeyRepeat_G;
	static Delegate<void, float> OnKeyRepeat_H;
	static Delegate<void, float> OnKeyRepeat_I;
	static Delegate<void, float> OnKeyRepeat_J;
	static Delegate<void, float> OnKeyRepeat_K;
	static Delegate<void, float> OnKeyRepeat_L;
	static Delegate<void, float> OnKeyRepeat_M;
	static Delegate<void, float> OnKeyRepeat_N;
	static Delegate<void, float> OnKeyRepeat_O;
	static Delegate<void, float> OnKeyRepeat_P;
	static Delegate<void, float> OnKeyRepeat_Q;
	static Delegate<void, float> OnKeyRepeat_R;
	static Delegate<void, float> OnKeyRepeat_S;
	static Delegate<void, float> OnKeyRepeat_T;
	static Delegate<void, float> OnKeyRepeat_U;
	static Delegate<void, float> OnKeyRepeat_V;
	static Delegate<void, float> OnKeyRepeat_W;
	static Delegate<void, float> OnKeyRepeat_X;
	static Delegate<void, float> OnKeyRepeat_Y;
	static Delegate<void, float> OnKeyRepeat_Z;

	
	static EKeyState KeyState_A;
	static EKeyState KeyState_D;
	static EKeyState KeyState_W;
	static EKeyState KeyState_S;
	static EKeyState KeyState_Space;
	static EKeyState KeyState_MouseLeft;
	static EKeyState KeyState_MouseRight;
	static EKeyState KeyState_MouseMiddle;



};

 /*
	定义一个名为“MoveForward”的轴映射

	当按键回调函数
	问题是 当按键按下，

	那些注册的W
	那些注册的A
	都不知道
 
	意味着，没有注册W的其他物体也会收到消息，问题是，并不能够区分委托里的方法哪个注册的W
	一个行为映射
		绑定多个按键
		多个按键操作同一个行为
	
	一个轴映射
		绑定多个按键
		每个按键返回不同的值

 */