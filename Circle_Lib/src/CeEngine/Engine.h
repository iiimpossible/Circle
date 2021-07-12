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


//��ģ������
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

//�޲��ػ�
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
 * @brief ������Ϊ������Ŀ����ӿڣ�����������ã���������ģʽ,��Ϣ������������շ��ͣ����������¼�
 * @detail ��ΪC++���Ա��������ڵ�ȷ�����ڱ�����ȷ���ģ�����̬���ӿ���Ҫ������ʱѰ�Һ�����ڵ㣬
 * ���²���ͨ�������������ķ�ʽ��ȷ��������ڵ㣬�Ӷ����²���ֱ���ڿ�ʹ���ߴ�ֱ�ӷ������Ա������
 * ��ˣ�ʹ���麯���������������һ����
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
	//�������

	/** @func ��ã����浱ǰģʽ*/
	static EEngineState GetEngineState();

	/** @func ������һ��Engine���󲢷���ָ��*/
	virtual Engine* CreateEngine();	

	/**
	 * @brief ��ʼ�����棬����GLFW��GLEW��ĳ�ʼ������������
	 * @detail ���������Ӧ������ʼ������
	 * @param aW ���ڿ��
	 * @param aH ���ڸ߶�
	 */
	virtual int InitEngine(int aW, int aH);

	/** @func ���ã���Ϸѭ��*/
	virtual void GameLoop();

	/** @func ���ã�ÿ��ѭ��������ʱ���Կ���֡�ʣ�Ŀǰ֡�ʼ��㲻̫�ᣬ�ù�ȥ֡��ʱ�������㵱ǰ֡��ʱ�䲻׼ȷ��*/
	virtual void SetSleepTimePerFrame();
public:
	//�������

	/** @func ���ã����ڴ�С*/
	virtual void ResizeCurrentWindow(int aWidth, int aHeight);
	/** @func ���ã�����λ��*/
	virtual void SetWidowPosition(int aX, int aW);	

	/** @func ��ã�������ָ��*/
	virtual CeGraphWindow* GetCEWindow();

	virtual HWND GetCeWindowHandle();


public:
	//��Ϸ�������

	/** @func ������ͨ����̬���ӿ��ȸ��µĹ���������̬������Ϸ����*/
	virtual void CreateGameObject(const string& aObjectType);

	virtual void DeleteGameObject(const char* aObjectName);


public:
	//��ȡ��Ϸ������Ϣ

	/** @func ��ã�ͨ�����ֲ��ø���Ϸ����ļ�����Ϣ*/
	virtual GeometryInfo GetGameObjectGeometryInfo(const string& aObjectName);

	/** @func ��ã���Ϸ��������б�*/
	virtual vector<string> GetGameObjectComponentList(const char* aObjectName);

	/** @func ��ã���ǰ������Ϸ�����б��������֣�*/
	virtual vector<string> GetCurrentSceneObjectNames();

	/** @func ��ã�ע�����Ϸ�������͵���������*/
	virtual vector<string> GetGameObjectTypeNames();

	/** @func ��ã���Ϸ��������*/
	virtual vector<string> GetGameObjectTypes();
public:
	//������Ϸ��������

	/** @func ���ã���Ϸ�������*/
	virtual void SetGameObjectMaterial(const char* aObjectName, const char* aMaterialName);

	/** @func ���ã���Ϸ����ļ�����Ϣ*/
	virtual void SetGameObjectGeometry(const char* aObjecName, float aX,float aY,float aZ, EGeometry aEGE = EGE_Pos);

	virtual void SetComponentCommand(const char* aObjecName, const char* aCompType, const char* aCommand);

	virtual void SetGameobjectName(const char* aOldName, const char* aNewName);
public:
	//��ȡ��Դ��Ϣ

	virtual vector<string> GetMaterialList();


public:
	//��Դ����

	virtual int LoadMaterial(const char* aPath);

	virtual int LoadModel(const char* aPath);

	virtual int LoadScene(const char* aPath);

	virtual int LoadSkybox(const char* aPath);

public:
	//��Դ����

	/**
	 * @breif ���浱ǰ����
	 * @detail ������ǰ�����е���Ϸ���壬Ȼ��ÿһ����Ϸ������������������������ȡ��Ҫ��Ϣ�Ա���
	 * Ŀǰ��Ҫ�������Ϣ����Ϸ�����transform���ԣ����֣����ͣ�������ø㣩
	 * ���ͣ���ȡͨ����̬���ͼ���ȡ�䶯̬���ͣ�Class type��,ȥ��ǰ���class�Ϳո�
	 * ���֣�ֱ�ӻ�ȡ������
	 * transform��������transform���
	 * д�뷽����ʵ��д��transform���Եķ�������������
	 */
	virtual void SaveScene(const char* aPath);

public:
	//�¼�	

	/** ��Ϊ����Ǹ���̬�⣬���Բ���ֱ�����û��������Ա*/
	virtual void BindEventOnDebugLog(IEngineEvent* aInterface);

	virtual void BindEventOnSceneRefresh(IEngineEvent* aInterface);

	virtual void BindEventOnGameobjectRename(IEngineEvent* aInterface);

	virtual void BindEnvetOnGameObjectCreate(IEngineEvent* aInterface);

public:
	//�¼�

	/** �����������־ʱִ�д��¼�*/
	static MemberFunc<IEngineEvent,void,string> mOnDebugLog;

	/** @breif ����������Ϸ�����б����ı䣨ɾ������ӡ��޸����֣�ʱ��ִ�д��¼�*/
	static MemberFunc<IEngineEvent,void,vector<string>> mOnSceneRefresh;

	static MemberFunc<IEngineEvent, void, string> mOnGameobjectRename;

	static MemberFunc<IEngineEvent, void, string> mOnGameObjectCreate;
public:
	//�������ľ�̬����

	inline static CeGraphWindow* GetCurrentWindow() { return mWindow; }

	static mutex mEngineMutex;
private:

	GameObject* CreateGameObjectByType(const char* aType);
private:

	static SceneManager* mScene;//��Ҫ�ͷ�
	static Renderer* mRenderer;
	static EEngineState EngineState;
	static CeGraphWindow* mWindow;//��Ҫ�ͷ�
	static ResourceManager* mResource;
	static InputManager* mInput;
	static LightManager* mLight;
	static bool mIsInit;
	static GLFWwindow* mGlWindow;
	static HWND mWinId;
	
};




