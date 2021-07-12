#include "Engine.h"
#include <iostream>
#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/glfw3native.h>
#include <CeCommon/OpenGL/CeWindow.h>
#include <CeCommon/OpenGL/CeDraw.h>
#include <CeCommon/Tool/CeUtility.h>

#include <CeEngine/Manager/InputManager.h>
#include <CeEngine/Manager/SceneManager.h>
#include <CeEngine/Manager/ScriptManager.h>
#include <CeEngine/Manager/LightManager.h>
#include <CeEngine/Object/ObjectFactory.h>
#include <CeEngine/Object/GameObject.h>
#include <CeEngine/Component/Component.h>
#include <CeEngine/Render/Renderer.h>
#include <CeEngine/Object/Scene.h>
#include <Celib_x64/src/CeCommon/Tool/Time.h>
#include <CeEngine/Manager/DebugLogManager.h>
#include <CeEngine/Manager/ResourceManager.h>
#include <Celib_x64/src/CeCommon/Tool/SceneLexer.h>
#include <Celib_x64/src/CeCommon/Tool/CeLoader.h>
		
#define PACKAGE_GL
//#define DIRECT_GL
//#if defined PACKAGE_GL
//
//#elif defined DIRECT_GL
//#endif

EEngineState Engine::EngineState = EEngineState::EES_3D;
bool  Engine::mIsInit = false;
CeGraphWindow* Engine::mWindow = nullptr;
SceneManager* Engine::mScene = nullptr;
Renderer* Engine::mRenderer = nullptr;
LightManager* Engine::mLight = nullptr;
InputManager* Engine::mInput = nullptr;
ResourceManager* Engine::mResource = nullptr;
MemberFunc<IEngineEvent, void, string> Engine::mOnDebugLog;
MemberFunc<IEngineEvent, void, vector<string>> Engine::mOnSceneRefresh;
MemberFunc<IEngineEvent, void, string> Engine::mOnGameobjectRename;
MemberFunc<IEngineEvent, void, string> Engine::mOnGameObjectCreate;

GLFWwindow*Engine::mGlWindow;
HWND Engine::mWinId;

mutex Engine::mEngineMutex;

Engine::Engine()
{
	
}

Engine::~Engine()
{
	delete mWindow; mWindow = nullptr;
	delete mScene; mScene = nullptr;
	delete mRenderer; mRenderer = nullptr;
	delete mResource; mResource = nullptr;
}


EEngineState Engine::GetEngineState()
{
	return EngineState;
}

Engine * Engine::CreateEngine()
{
	return new Engine();
}


//void Engine::InitEngine(int aW, int aH,OnWindowSizeChange aP)
//{
//	//创建Opengl窗口并初始化两个三方库
//	mWindow = new CeGraphWindow(aW,aH);
//
//	//开启Opengl的一些功能
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	//gl回调函数
//	glfwSetFramebufferSizeCallback(mWindow->GetGLFWWindow(), aP);
//	
//	//初始化引擎模块
//	mScene = new SceneManager();
//
//	//引擎初始化判定
//	mIsInit = true;
//}

int Engine::InitEngine(int aW, int aH)
{

#if defined PACKAGE_GL
	//初始化OpenGL的第三方库	
	int tag_window = CeGraphWindow::InitWindowAPI();
	//创建Opengl窗口
	mWindow = new CeGraphWindow(aW, aH);
	int tag_draw = CeGraphDrawing::InitDrawAPI();
	if (!tag_window || !tag_draw)
	{
		DebugLogManager::DebugOutput("Engine", EDebugTag::EDT_ERROR, { "Init windowAPI or init drawAPI exception." });
		CeGraphWindow::Terminate();//关闭失败窗口
	}
#elif defined DIRECT_GL
	int tag = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//版本设置，大版本号和和小版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//opengl 核心配置
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);//窗口是否可以改变大小
	//glfwWindowHint(GLFW_SAMPLES, 8);//开启多重采样，没用？
	mGlWindow = glfwCreateWindow(800, 600, "GraphicWindow", nullptr, nullptr);
	mWinId = glfwGetWin32Window(mGlWindow);	//获得图形窗口的window句柄	
	glfwMakeContextCurrent(mGlWindow);//使得某个图形窗口为当前上下文处理的窗口
	

	glewExperimental = true;
	unsigned a = glewInit();

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	//glGenVertexArrays(1,&a);
	//glGenFramebuffers(1,&a);
#endif // DERECTGL
	

	//初始化引擎模块
	mScene = new SceneManager();	

	mResource = new ResourceManager();

	mRenderer = new Renderer();
	
	mInput = InputManager::CreateInputManager();

	mLight = new LightManager();
	//引擎初始化判定
	mIsInit = true;
	return 1;
}

void Engine::GameLoop()
{
	if (!mIsInit) return;

	//资源加载是第一位的，渲染器中有依赖资源
	mResource->Start();

	mScene->Start();

	//渲染器依赖资源管理器中的一些纹理和着色器
	mRenderer->Start();	
	

#if defined PACKAGE_GL
	while (!glfwWindowShouldClose(mWindow->GetGLFWWindow()))
#elif defined DIRECT_GL
	while (!glfwWindowShouldClose(mGlWindow))
#endif	
	{		
		//Opengl事件循环
		glfwPollEvents();		
		//将前台帧缓冲填充固定色
		glClearColor(0.2f, 0.2f, 0.2f,1.0f);
		//清除后台帧缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//同时清除颜色和深度缓冲
		//场景更新
		mScene->Update();		
		
		mLight->Update();		//渲染
		
		mRenderer->Update();
		
#if defined PACKAGE_GL
		glfwSwapBuffers(mWindow->GetGLFWWindow());//双缓冲交换	
#elif defined DIRECT_GL
		glfwSwapBuffers(mGlWindow);//双缓冲交换	
#endif

		//交换前台后台帧缓冲
		
		//性能限制
		Sleep(3);
		//计算1秒内的平均帧率
		CeTimer::CaculateAverageDeletatime();
	}
	glfwTerminate();
}

CeGraphWindow* Engine::GetCEWindow()
{
	return mWindow;
}

HWND Engine::GetCeWindowHandle()
{
#if defined PACKAGE_GL
	if (!mWindow)
		return 0;
	else
		return mWindow->GetWinHandle();
#elif defined DIRECT_GL
	return mWinId;
#endif // DERECT_GL

	
}


void Engine::SetSleepTimePerFrame()
{
}

void Engine::ResizeCurrentWindow(int aWidth, int aHeight)
{

#if defined PACKAGE_GL
	mWindow->ResizeWindow(aWidth, aHeight);
#elif defined DIRECT_GL
	glfwSetWindowSize(mGlWindow, aWidth, aHeight);
	glViewport(0, 0, aWidth, aHeight);
#endif // DERECT_GL
	
	
}

void Engine::SetWidowPosition(int aX, int aW)
{
}

void Engine::CreateGameObject(const string & aObjectType)
{  
	mScene->GetCurrentScene()->InsertGameObject(aObjectType);
}

void Engine::DeleteGameObject(const char * aObjectName)
{
	mScene->GetCurrentScene()->DeleteGameObject(aObjectName);
}

GameObject * Engine::CreateGameObjectByType(const char * aType)
{
	auto obj = ObjectFactory::GetObjectInstance(aType);
	if (!obj)
	{
		DebugLogManager::DebugOutput("Enging", EDebugTag::EDT_Log, { "Create gameObject failed." });
		return nullptr;
	}
	mScene->InsertGameObejct(obj);
	//DebugLogManager::DebugOutput("Enging", EDebugTag::EDT_Log, { "Objecg name is: ",obj->GetName() });
	return obj;
}


GeometryInfo Engine::GetGameObjectGeometryInfo(const string & aObjectName)
{
	 GameObject* obj = mScene->GetCurrentScene()->FindGameObject(aObjectName);
	 if (obj)
	 {
		 Vec3 pos = obj->GetObjectPosition();
		 Vec3 rot = obj->GetObjectRotation();
		 Vec3 size = obj->GetObjectSize();
		 return { pos.x,pos.y ,pos.z ,rot.x ,rot.y ,rot.z ,size.x ,size.y ,size.z };
	 }
	 else
		 return {};	 
}


vector<string> Engine::GetGameObjectComponentList(const char* aObjectName)
{
	//获得当前场景。其实内存中只有一个场景，更换场景需要重新加载
	Scene*  scene = mScene->GetCurrentScene();
	GameObject* obj = scene->FindGameObject(aObjectName);
	if(obj)return obj->GetComponentList();
	else return {};	
}

vector<string> Engine::GetCurrentSceneObjectNames()
{
	auto tP = mScene->GetCurrentScene();
	if (tP)
		return tP->GetGameObjectNameList();
	else
		return {};	
}

vector<string> Engine::GetGameObjectTypeNames()
{
	return ObjectFactory::GetGameObjectTypeNames();
}

void Engine::SetGameObjectMaterial(const char * aObjectName, const char * aMaterialName)
{
	GameObject* object = mScene->GetCurrentScene()->FindGameObject(aObjectName);
	if(object)
		object->SetMaterial(aMaterialName);
}

void Engine::SetGameObjectGeometry(const char * aObjectName, float aX, float aY, float aZ, EGeometry aEGE)
{
	GameObject* obj = mScene->GetCurrentScene()->FindGameObject(aObjectName);
	if (!obj)
	{
		DebugLogManager::DebugOutput("Enging", EDebugTag::EDT_ERROR,{ " Unable to find [", aObjectName, "]." });
		return;
	}
	switch (aEGE)
	{
	case EGE_Pos:
		obj->SetObejctPosition(Vec3(aX,aY,aZ));
		break;
	case EGE_Rot:
		obj->SetObjectRotation(Vec3(aX, aY, aZ));
		break;
	case EGE_Size:
		obj->SetObjectSize(Vec3(aX, aY, aZ));
		break;
	default:
		break;
	}
}

void Engine::SetComponentCommand(const char * aObjecName, const char * aCompType, const char * aCommand)
{	
	GameObject* obj= mScene->GetCurrentScene()->FindGameObject(aObjecName);
	if (!obj) return;
	Component* comp = obj->GetComponent(aCompType);
	if (!comp) return;
	comp->SetCommand(aCommand);	 
}

void Engine::SetGameobjectName(const char * aOldName, const char * aNewName)
{
	mScene->GetCurrentScene()->SetGameobjctName(aOldName, aNewName);
}

vector<string> Engine::GetMaterialList()
{
	return ResourceManager::GetMaterialList();
}

vector<string> Engine::GetGameObjectTypes()
{
	return ObjectFactory::GetGameObjectTypeNames();
}

void Engine::BindEventOnDebugLog(IEngineEvent* aInterface)
{
	mOnDebugLog.Init(aInterface, &IEngineEvent::DebugLogCallback);
}

void Engine::BindEventOnSceneRefresh(IEngineEvent* aInterface)
{
	mOnSceneRefresh.Init(aInterface, &IEngineEvent::SceneRefreshCallback);
}

void Engine::BindEventOnGameobjectRename(IEngineEvent * aInterface)
{
	mOnGameobjectRename.Init(aInterface, &IEngineEvent::GameObjectRenameCallback);
}

void Engine::BindEnvetOnGameObjectCreate(IEngineEvent * aInterface)
{
	mOnGameObjectCreate.Init(aInterface, &IEngineEvent::GameObjectCreateCallback);
}



int Engine::LoadMaterial(const char * aPath)
{
	return ResourceManager::LoadMaterial(aPath);
}

int Engine::LoadModel(const char * aPath)
{
	return ResourceManager::LoadStaticMesh(aPath);
}

int Engine::LoadScene(const char * aPath)
{
	Scene* cur_scene = mScene->GetCurrentScene();
	cur_scene->ClearScene();
	vector<ObjectInitInfo> info;
	int res = SceneLexer::Lexer(CeLoader::LoadText(aPath), &info);
	//DebugLogManager::DebugOutput("Engine", EDebugTag::EDT_ERROR, { SceneLexer::GetDebugLog() });
	if (!res)
	{
		DebugLogManager::DebugOutput("Engine", EDebugTag::EDT_ERROR, {SceneLexer::GetDebugLog()});
		return 0;
	}	
	
	//mEngineMutex.lock();
	for (auto val:info)
	{				
		GameObject* object = CreateGameObjectByType(val.type.c_str());
		if (object)
		{
			object->SetName(val.name);
			for (int i = 0; i < val.comp_command.size(); i++)
			{
				Component* comp = object->GetComponent(val.comp_command[i].first);
				if (!comp)
				{
					DebugLogManager::DebugOutput("Engine", EDebugTag::EDT_ERROR, { "Component not exist. type is ", val.comp_command[i].first });
					continue;
				}
				comp->SetCommand(val.comp_command[i].second);
			}

		}
		else
		{
			DebugLogManager::DebugOutput("Engine", EDebugTag::EDT_ERROR, {"Object ceate failed, perhaps type not exist." });
			return 0;
		}
	
	}
	//mEngineMutex.unlock();
	return 1;
}

int Engine::LoadSkybox(const char * aPath)
{
	return ResourceManager::LoadTextureCube(aPath);;
}

void Engine::SaveScene(const char* aPath)
{
	Scene* cur_scene = mScene->GetCurrentScene();

	//遍历所有游戏物体
		//遍历所有组件
			//获得当前命令
		//生成一个游戏信息
	//生成整个场景信息

	string str = SceneLexer::WirteSceneScript("null",cur_scene->GetObjectInitInfo());
	DebugLogManager::DebugOutput("Engine", EDebugTag::EDT_Log, {SceneLexer::GetDebugLog() });
	CeUtility::WriteText(aPath, str);
	//写入场景

}

