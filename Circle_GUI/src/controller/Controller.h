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
 * @brief ��Ϸ���弸����Ϣ
 */
struct GTInfo
{
	QVector3D Pos;
	QVector3D Rot;
	QVector3D Size;
};

/**
 * @brief ����������ṩһЩ�ӿڸ����û����棬���������ݣ�����������ײ��һЩ���ܣ�������̬������ͷ�
 * @detail ͨ������ʱ�����ͷŶ�̬�����ʵ�ֲ��������������¸���C++�ű�
 * �����������API����װ����ϵ������
 * �¼���֡���ƣ���ȡ��Ϣ��
 * �¼�����ɾ����
 *
 */
class Controller:public QWidget,public IEngineEvent
{
	Q_OBJECT
public:
	explicit Controller(QWidget* widget = Q_NULLPTR);
	~Controller();
public:
	//OpenGL����
	 HWND GetGLWindowHandle();

	void ResizeWindow();	

	void ExecGameLoop();

public:
	//��̬�����

	static void LoadDynamicLibrary();

	static void UnLoadDynamicLibrary();

signals:
	//�ź�

	/** �źţ�������Ϸ����*/
	void OnGameObjecetCreate(QString aObjectName);

	/** �źţ������Ѽ���*/
	void OnMaterialLoad(const QString& aMaterialName);

	/** �źţ�ģ���Ѽ���*/
	void OnModelLoad(const QString& aModelName);

	/** �źţ������Ѽ��� */
	void OnSceneLoad(const QVector<QString>& aObjectNameList);

	void OnSceneLoadMaterialRefresh(const QVector<QString>& aMaterialList);

	void OnGameobjectRenamed(const QString& aNewName);
public slots:

	/** �ۺ�������Ϸ�����б�������������ò��ʡ�ɾ����Ϸ����*/
	void SlotObjectListAction(EObjectListAction aAction,
		const QString& aObjectName,
		const QString& aMaterialName);

	/** �ۺ�����������Ϸ����Ĳ���*/
	void SlotSetObjctMaterial(const QString& aObjectName, const QString& aMaterialName);

	/** �ۺ���������ģ��*/
	void SlotLoadModel(const QString& aPath);

	/** �ۺ��������س��� */
	void SlotLoadScene(const QString& aPath);

	void SlotLoadSkybox(const QString& aPath);

	/** �ۺ�������Ϸ���弸����Ϣ�ı�*/
	void SlotObjectTransformModified(const QString& aObjectName, const QVector3D& aVec, int aTag);

	/** �ۺ������ı�ͼ�δ��ڵĴ�С*/
	void SlotResizeCurrentGraphicWindow(int aWidth, int aHeight);

	void SlotSendComponentCommand(const QString& aObjectName, const QString& aCompType, const QString& aCommand);

	void SlotDeleteGameObject(const QString& aObjectName);

	void SlotSetObjectName(const QString& aOldName, const QString& aNewName);
public:
	//��Ϣ����

	/** ͨ�������������壨���ִ��浽map�У���Ӧһ������������*/
	static GTInfo GetGameObjectGTInfo(const QString& aName);
	
	/** ��ȡ�������������б��Բ������ֶ�Ӧ�������Ϣ��һЩ�¼����������ĸ��б�*/			
	static QVector<QString> GetGameObjectNameList();

	static QVector<QString> GetObjectComponentList(const QString& aName);

	const QVector<QString> GetObjectTypes();

public:
	//��Ϸ�������

	void CreateObject(const QString& aObjectType);

	//�ı���Ϸ���弸��λ��
	//void 

public:
	//һЩ���ܷ���

	/** @brief ��������״̬*/
	static void EngineConfig();

	/** @brief ����ģ��*/
	void LoadModel(const QUrl& aPath);

	/** @brief ������Ƶ*/
	void LoadAudio(const QString& aPath);

	/** @brief ��������*/
	void LoadTexture(const QString& aPath);

	/** @brief ���ز���*/
	void LoadMaterial(const QUrl& aPath);

	/** @brief ���س���*/
	void LoadScene(const QUrl& aPath);

	void LoadSkybox(const QUrl& aPath);

	/** @brief ���泡��*/
	void SaveScene(const QUrl& aPath);


public:
	//ʵ�ֵĴ�����̳�������ӿڣ������¼���

	/** ��־��Ϣ����Ļص�����*/
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