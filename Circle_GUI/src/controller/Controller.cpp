#include "Controller.h"
#include <qdebug.h>
#include <qopengl.h>
#include <qopenglwidget.h>

#include <src/CeCommon/OpenGL/CeWindow.h>
#include <pch.h>
//#include <Source/CCEngine/Engine.h>

#include "../../ShaderViewer.h"
#include "../../../src/widget/FunctionalWidget/Console.h"


//这里将包含所有引擎源文件
ShaderViewer* Controller::mMainWidget = nullptr;
Engine* Controller::mEngine = nullptr;
Controller::Controller(QWidget* widget):
	QWidget(widget)
{
	//动态库加载，释放需要另外写一个函数，因为不一定是析构时释放的

	//这个方法是在动态库的pch文件里的，因为不能直接使用Engine类的构造函数生成其实例，只能用c风格方法
	mEngine = InitCircleEngine();

	mEngine->BindEventOnDebugLog(this);

	mEngine->BindEventOnSceneRefresh(this);

	mEngine->BindEventOnGameobjectRename(this);

	mEngine->BindEnvetOnGameObjectCreate(this);

	//启动引擎，包括生成图形窗口
	int tag = mEngine->InitEngine(800,600);


	//简单游戏物体类反射支持
	mGameobjectTypeNames = mEngine->GetGameObjectTypeNames();
	

	
	
}

Controller::~Controller()
{

	delete mEngine;
	mEngine = 0;
}

HWND Controller::GetGLWindowHandle()
{
	//GameObject* g = new GameObject();
	//return mEngine->GetCEWindow()->GetWinHandle();
	return mEngine->GetCeWindowHandle();
	//return nullptr;
}

void Controller::ResizeWindow()
{
	
}



void Controller::ExecGameLoop()
{
	if (!mEngine) return;
	mEngine->GameLoop();
}

void Controller::LoadDynamicLibrary()
{
	WCHAR wszClassName[256];
	LPCWSTR a = wszClassName;
	HMODULE hmodule = ::LoadLibrary(wszClassName);
}

void Controller::UnLoadDynamicLibrary()
{
	HMODULE hmodule = 0;
	FreeLibrary(hmodule);
}

void Controller::SlotObjectListAction(EObjectListAction aAction, 
	const QString& aObjectName, 
	const QString& aMaterialName)
{
	switch (aAction)
	{
	case ELA_SetMaterial://设置材质命令
		break;
	case ELA_Delete://删除改物体命令
		break;
	default:
		break;
	}
}



GTInfo Controller::GetGameObjectGTInfo(const QString& aName)
{
	 auto val = mEngine->GetGameObjectGeometryInfo(aName.toStdString()); 
	 return { QVector3D(val.PosX,val.PosY,val.PosZ),
		 QVector3D(val.RotX,val.RotY,val.RotZ),
		 QVector3D(val.SizeX,val.SizeY,val.SizeZ) };
}



QVector<QString> Controller::GetGameObjectNameList()
{
	QVector<QString> str;
	for (auto val : mEngine->GetCurrentSceneObjectNames())
	{
		str.push_back(QString::fromStdString(val));
	}
	return str;
}

QVector<QString> Controller::GetObjectComponentList(const QString & aName)
{
	QVector<QString> str;
	for (auto val : mEngine->GetGameObjectComponentList(aName.toStdString().c_str()))
	{
		str.push_back(QString::fromStdString(val));
	}
	return str;
}


void Controller::CreateObject(const QString & aObjectType)
{
	mEngine->CreateGameObject(aObjectType.toStdString());
	//emit OnGameObjecetCreate(QString::fromStdString(name));
}

void Controller::SlotSetObjctMaterial(const QString & aObjectName, const QString & aMaterialName)
{
	mEngine->SetGameObjectMaterial(aObjectName.toStdString().c_str(),
		aMaterialName.toStdString().c_str());
}

void Controller::SlotLoadModel(const QString & aPath)
{
	mEngine->LoadModel(aPath.toStdString().c_str());
}

void Controller::SlotLoadScene(const QString & aPath)
{
	mEngine->LoadScene(aPath.toStdString().c_str());
}

void Controller::SlotLoadSkybox(const QString & aPath)
{
}

void Controller::SlotObjectTransformModified(const QString & aObjectName, const QVector3D & aVec, int aTag)
{	
	switch (aTag)
	{
	case 1:
		mEngine->SetGameObjectGeometry(aObjectName.toStdString().c_str(), aVec.x(), aVec.y(), aVec.z(), EGeometry::EGE_Pos);
		break;
	case 2:
		mEngine->SetGameObjectGeometry(aObjectName.toStdString().c_str(), aVec.x(), aVec.y(), aVec.z(), EGeometry::EGE_Rot);
		break;
	case 3:
		mEngine->SetGameObjectGeometry(aObjectName.toStdString().c_str(), aVec.x(), aVec.y(), aVec.z(), EGeometry::EGE_Size);
		break;
	default:
		break;
	}
}

void Controller::SlotResizeCurrentGraphicWindow(int aWidth, int aHeight)
{
	mEngine->ResizeCurrentWindow(aWidth, aHeight);
}

void Controller::SlotSendComponentCommand(const QString & aObjectName, const QString & aCompType, const QString & aCommand)
{
	mEngine->SetComponentCommand(aObjectName.toStdString().c_str(), 
		aCompType.toStdString().c_str(), aCommand.toStdString().c_str());
}

void Controller::SlotDeleteGameObject(const QString & aObjectName)
{
	mEngine->DeleteGameObject(aObjectName.toStdString().c_str());
}

void Controller::SlotSetObjectName(const QString & aOldName, const QString & aNewName)
{
	mMainWidget->mConSole->AddTextBack(aOldName + " space "+ aNewName);
	mEngine->SetGameobjectName(aOldName.toStdString().c_str(), aNewName.toStdString().c_str());
}

const QVector<QString> Controller::GetObjectTypes()
{
	QVector<QString> object_types;
	for (auto val : mEngine->GetGameObjectTypes())
	{
		object_types.push_back(QString::fromStdString(val));
	}
	return object_types;
}

void Controller::SaveScene(const QUrl& aPath)
{
	QString str = aPath.toLocalFile();
	mEngine->SaveScene(str.toStdString().c_str());
}


/*===========================================================================================================*/


void Controller::DebugLogCallback(string aLog)
{
	if(mMainWidget)
		mMainWidget->mConSole->AddTextBack(QString::fromStdString(aLog));
}

void Controller::SceneRefreshCallback(vector<string> aNames)
{
	QVector<QString> object_names;
	//不能
	for (auto val : mEngine->GetCurrentSceneObjectNames())
	{
		object_names.push_back(QString::fromStdString(val));
	}
	emit OnSceneLoad(object_names);
}

void Controller::GameObjectRenameCallback(string aNewName)
{
	//mMainWidget->mConSole->AddTextBack(QString::fromStdString(aNewName));
	emit OnGameobjectRenamed(QString::fromStdString(aNewName));
}

void Controller::GameObjectCreateCallback(string aName)
{
	emit OnGameObjecetCreate(QString::fromStdString(aName));
}


/*===========================================================================================================*/


void Controller::LoadModel(const QUrl & aPath)
{
	QString t = aPath.toLocalFile();
	int res = mEngine->LoadModel(t.toStdString().c_str());
	if (res) emit OnModelLoad(aPath.fileName());
}

void Controller::LoadMaterial(const QUrl& aPath)
{
	QString t = aPath.toLocalFile();
	//int res = mEngine->LoadMaterial(aPath.toString().toStdString().c_str());
	int res = mEngine->LoadMaterial(t.toStdString().c_str());
	if (res > 0) emit OnMaterialLoad(aPath.fileName());
}

void Controller::LoadScene(const QUrl & aPath)
{
	QString t = aPath.toLocalFile();
	//int res = mEngine->LoadMaterial(aPath.toString().toStdString().c_str());
	int res = mEngine->LoadScene(t.toStdString().c_str());
	
	QVector<QString> material_names;	
	
	for (auto val : mEngine->GetMaterialList())
	{
		material_names.push_back(QString::fromStdString(val));
	}

	if (res)
	{	
		emit OnSceneLoadMaterialRefresh(material_names);
	}
	
}

void Controller::LoadSkybox(const QUrl & aPath)
{
	QString str = aPath.toLocalFile();
	mEngine->LoadSkybox(str.toStdString().c_str());
}
	
	
