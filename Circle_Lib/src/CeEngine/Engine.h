#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <mutex>
#include <Celib_x64/src/CeCommon/Tool/Delegate.h>
#include <Celib_x64/pch.h>
//#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
using std::string;
using std::vector;
using std::mutex;
using namespace CircleEngine;


class CeGraphWindow;
class Renderer;
struct GLFWwindow;
class SceneManager;
class GameObject;
class ResourceManager;
class InputManager;
class LightManager;


enum EOperation
{
	EOP_Delete = 1,
	EOP_Create = 2,
	EOP_Modify = 3,
	EOP_LOAD = 4
};

enum EGeometry
{
	EGE_Pos = 1,
	EGE_Rot = 2,
	EGE_Size = 3
};


enum class EEngineState 
{
	EES_3D,
	EES_2D,
	EES_ShaderDevelop 
};



struct GeometryInfo
{
	float PosX;
	float PosY;
	float PosZ;
	float RotX;
	float RotY;
	float RotZ;
	float SizeX;
	float SizeY;
	float SizeZ;
};


//主模板声明
template<typename ObjType, typename ReturnType, typename ...ParamType>
class MemberFunc
{
	typedef ReturnType(ObjType::*Func)(ParamType...);
public:

	MemberFunc(ObjType* aObj = 0, Func aFunc = 0) { mObj = aObj; mfunc = aFunc; }
	void Init(ObjType* aObj, Func aFunc) { mObj = aObj; mfunc = aFunc; }
	ReturnType Invoke(ParamType... aParams) { return (mObj->*mfunc)(aParams...); }
private:
	ObjType* mObj;
	Func mfunc;
};

//无参特化
template<typename ObjType, typename ReturnType>
class MemberFunc<ObjType, ReturnType>
{
	typedef ReturnType(ObjType::*Func)();
public:
	MemberFunc(ObjType* aObj = 0, Func aFunc = 0) { mObj = aObj; mfunc = aFunc; }
	ReturnType Invoke() { return (mObj->*mfunc)(); }
private:
	ObjType* mObj;
	Func mfunc;
};



class IEngineEvent
{
public:
	virtual void DebugLogCallback(string) = 0;

	virtual void SceneRefreshCallback(vector<string>) = 0;

	virtual void GameObjectRenameCallback(string) = 0;

	virtual void GameObjectCreateCallback(string) = 0;
};


/**
 * @brief 该类作为引擎核心库对外接口，引擎基础配置，管理引擎模式,信息交互，命令接收发送，定义引擎事件
 * @detail 因为C++类成员函数的入口点确认是在编译期确定的，而动态链接库需要在运行时寻找函数入口点，
 * 导致不能通过类名：方法的方式来确定函数入口点，从而导致不能直接在库使用者处直接访问类成员函数。
 * 因此，使用虚函数（虚表）来避免这一限制
 * @event 
 */
class CELIB_CLASS_API Engine
{
	friend InputManager;
private:

	typedef void(*OnWindowSizeChange)(GLFWwindow*, int, int) ;	
public:

	Engine();
	virtual ~Engine();
public:
	//引擎相关

	/** @func 获得：引擎当前模式*/
	static EEngineState GetEngineState();

	/** @func 创建：一个Engine对象并返回指针*/
	virtual Engine* CreateEngine();	

	/**
	 * @brief 初始化引擎，包括GLFW和GLEW库的初始化，创建窗口
	 * @detail 创建引擎后应立即初始化引擎
	 * @param aW 窗口宽度
	 * @param aH 窗口高度
	 */
	virtual int InitEngine(int aW, int aH);

	/** @func 调用：游戏循环*/
	virtual void GameLoop();

	/** @func 设置：每个循环的休眠时间以控制帧率（目前帧率计算不太会，用过去帧的时间来计算当前帧的时间不准确）*/
	virtual void SetSleepTimePerFrame();
public:
	//窗口相关

	/** @func 设置：窗口大小*/
	virtual void ResizeCurrentWindow(int aWidth, int aHeight);
	/** @func 设置：窗口位置*/
	virtual void SetWidowPosition(int aX, int aW);	

	/** @func 获得：窗口类指针*/
	virtual CeGraphWindow* GetCEWindow();

	virtual HWND GetCeWindowHandle();


public:
	//游戏物体操作

	/** @func 创建：通过动态链接库热更新的工厂方法动态生成游戏物体*/
	virtual void CreateGameObject(const string& aObjectType);

	virtual void DeleteGameObject(const char* aObjectName);


public:
	//获取游戏物体信息

	/** @func 获得：通过名字查获得该游戏物体的几何信息*/
	virtual GeometryInfo GetGameObjectGeometryInfo(const string& aObjectName);

	/** @func 获得：游戏物体组件列表*/
	virtual vector<string> GetGameObjectComponentList(const char* aObjectName);

	/** @func 获得：当前场景游戏物体列表（返回名字）*/
	virtual vector<string> GetCurrentSceneObjectNames();

	/** @func 获得：注册的游戏物体类型的类型名字*/
	virtual vector<string> GetGameObjectTypeNames();

	/** @func 获得：游戏物体类型*/
	virtual vector<string> GetGameObjectTypes();
public:
	//设置游戏物体属性

	/** @func 设置：游戏物体材质*/
	virtual void SetGameObjectMaterial(const char* aObjectName, const char* aMaterialName);

	/** @func 设置：游戏物体的几何信息*/
	virtual void SetGameObjectGeometry(const char* aObjecName, float aX,float aY,float aZ, EGeometry aEGE = EGE_Pos);

	virtual void SetComponentCommand(const char* aObjecName, const char* aCompType, const char* aCommand);

	virtual void SetGameobjectName(const char* aOldName, const char* aNewName);
public:
	//获取资源信息

	virtual vector<string> GetMaterialList();


public:
	//资源加载

	virtual int LoadMaterial(const char* aPath);

	virtual int LoadModel(const char* aPath);

	virtual int LoadScene(const char* aPath);

	virtual int LoadSkybox(const char* aPath);

public:
	//资源保存

	/**
	 * @breif 保存当前场景
	 * @detail 遍历当前场景中的游戏物体，然后每一个游戏物体遍历其组件（将来），获取重要信息以保存
	 * 目前需要保存的信息：游戏物体的transform属性，名字，类型（这个不好搞）
	 * 类型：获取通过动态类型检测获取其动态类型（Class type）,去掉前面的class和空格
	 * 名字：直接获取其名字
	 * transform：访问其transform组件
	 * 写入方法：实现写入transform属性的方法，其他容易
	 */
	virtual void SaveScene(const char* aPath);

public:
	//事件	

	/** 因为这个是个动态库，所以不能直接在用户访问类成员*/
	virtual void BindEventOnDebugLog(IEngineEvent* aInterface);

	virtual void BindEventOnSceneRefresh(IEngineEvent* aInterface);

	virtual void BindEventOnGameobjectRename(IEngineEvent* aInterface);

	virtual void BindEnvetOnGameObjectCreate(IEngineEvent* aInterface);

public:
	//事件

	/** 当输出调试日志时执行此事件*/
	static MemberFunc<IEngineEvent,void,string> mOnDebugLog;

	/** @breif 当场景的游戏物体列表发生改变（删除、添加、修改名字）时，执行此事件*/
	static MemberFunc<IEngineEvent,void,vector<string>> mOnSceneRefresh;

	static MemberFunc<IEngineEvent, void, string> mOnGameobjectRename;

	static MemberFunc<IEngineEvent, void, string> mOnGameObjectCreate;
public:
	//不导出的静态方法

	inline static CeGraphWindow* GetCurrentWindow() { return mWindow; }

	static mutex mEngineMutex;
private:

	GameObject* CreateGameObjectByType(const char* aType);
private:

	static SceneManager* mScene;//需要释放
	static Renderer* mRenderer;
	static EEngineState EngineState;
	static CeGraphWindow* mWindow;//需要释放
	static ResourceManager* mResource;
	static InputManager* mInput;
	static LightManager* mLight;
	static bool mIsInit;
	static GLFWwindow* mGlWindow;
	static HWND mWinId;
	
};




