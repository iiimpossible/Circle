/**
 * @file		Contorller.h
 * @data		2021.1.20
 * @author		iiimpossible
 * @copyright	Copyright (c) 2021 iiimpossible. All Rights Reserved.
 */
#pragma once
#include <iostream>
#include <string>
#include <qvector.h>
#include <Windows.h>
#include <qwidget.h>
#include <qstring.h>
#include <list>
#include <qlist.h>
#include <qvector3d.h>
#include <ShaderViewer/src/common/MyCommon.h>
#include <src/CeEngine/Engine.h>

using std::string;
class Engine;
struct GLFWwindow;
class ShaderViewer;

struct UserCommand
{

};

/**
 * @brief 游戏物体几何信息
 */
struct GTInfo
{
	QVector3D Pos;
	QVector3D Rot;
	QVector3D Size;
};

/**
 * @brief 这个控制器提供一些接口给到用户界面，更新其数据，并控制引擎底层的一些功能，包括动态库加载释放
 * @detail 通过运行时加载释放动态库可以实现不重启程序的情况下更新C++脚本
 * 将引擎的所有API都封装到该系列类中
 * 事件：帧绘制（读取信息）
 * 事件：增删物体
 *
 */
class Controller:public QWidget,public IEngineEvent
{
	Q_OBJECT
public:
	explicit Controller(QWidget* widget = Q_NULLPTR);
	~Controller();
public:
	//OpenGL窗口
	 HWND GetGLWindowHandle();

	void ResizeWindow();	

	void ExecGameLoop();

public:
	//动态库操作

	static void LoadDynamicLibrary();

	static void UnLoadDynamicLibrary();

signals:
	//信号

	/** 信号：创建游戏物体*/
	void OnGameObjecetCreate(QString aObjectName);

	/** 信号：材质已加载*/
	void OnMaterialLoad(const QString& aMaterialName);

	/** 信号：模型已加载*/
	void OnModelLoad(const QString& aModelName);

	/** 信号：场景已加载 */
	void OnSceneLoad(const QVector<QString>& aObjectNameList);

	void OnSceneLoadMaterialRefresh(const QVector<QString>& aMaterialList);

	void OnGameobjectRenamed(const QString& aNewName);
public slots:

	/** 槽函数：游戏物体列表操作，包括设置材质、删除游戏物体*/
	void SlotObjectListAction(EObjectListAction aAction,
		const QString& aObjectName,
		const QString& aMaterialName);

	/** 槽函数：设置游戏物体的材质*/
	void SlotSetObjctMaterial(const QString& aObjectName, const QString& aMaterialName);

	/** 槽函数：加载模型*/
	void SlotLoadModel(const QString& aPath);

	/** 槽函数：加载场景 */
	void SlotLoadScene(const QString& aPath);

	void SlotLoadSkybox(const QString& aPath);

	/** 槽函数：游戏物体几何信息改变*/
	void SlotObjectTransformModified(const QString& aObjectName, const QVector3D& aVec, int aTag);

	/** 槽函数：改变图形窗口的大小*/
	void SlotResizeCurrentGraphicWindow(int aWidth, int aHeight);

	void SlotSendComponentCommand(const QString& aObjectName, const QString& aCompType, const QString& aCommand);

	void SlotDeleteGameObject(const QString& aObjectName);

	void SlotSetObjectName(const QString& aOldName, const QString& aNewName);
public:
	//信息交互

	/** 通过名字索引物体（名字储存到map中，对应一个场景物体编号*/
	static GTInfo GetGameObjectGTInfo(const QString& aName);
	
	/** 获取到场景中名字列表，以查找名字对应物体的信息，一些事件发生后会更改该列表*/			
	static QVector<QString> GetGameObjectNameList();

	static QVector<QString> GetObjectComponentList(const QString& aName);

	const QVector<QString> GetObjectTypes();

public:
	//游戏物体操作

	void CreateObject(const QString& aObjectType);

	//改变游戏物体几何位置
	//void 

public:
	//一些功能方法

	/** @brief 配置引擎状态*/
	static void EngineConfig();

	/** @brief 加载模型*/
	void LoadModel(const QUrl& aPath);

	/** @brief 加载音频*/
	void LoadAudio(const QString& aPath);

	/** @brief 加载纹理*/
	void LoadTexture(const QString& aPath);

	/** @brief 加载材质*/
	void LoadMaterial(const QUrl& aPath);

	/** @brief 加载场景*/
	void LoadScene(const QUrl& aPath);

	void LoadSkybox(const QUrl& aPath);

	/** @brief 保存场景*/
	void SaveScene(const QUrl& aPath);


public:
	//实现的从引擎继承来的虚接口，用于事件绑定

	/** 日志信息输出的回调函数*/
	virtual void DebugLogCallback(string aLog) override;

	virtual void SceneRefreshCallback(vector<string> aNames)override;

	virtual void GameObjectRenameCallback(string) override;

	virtual void GameObjectCreateCallback(string) override;

public:
	static ShaderViewer* mMainWidget;

private:
	
	static Engine* mEngine;

	vector<string> mGameobjectTypeNames;

	




};