#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShaderViewer.h"

class Controller;
class InspectorWidget;
class MyDockWidget;
class ConsoleWidget;
class ObjectListWidget;
class FileExploreWidget;
class MyCenterWidget;

class ShaderViewer : public QMainWindow
{
    Q_OBJECT

public:
    ShaderViewer(QWidget *parent = Q_NULLPTR);

	void ShowWidgets();

	/** ���Ӹ����źŲۣ�����ָ��δ��ʼ��*/	
	void PlugSocket();

	void RightButtonMenu();

	inline void SetController(Controller* aController) { mController = aController; }



private:
    Ui::ShaderViewerClass ui;
	InspectorWidget* mInspector;
	MyDockWidget* dock;
	MyCenterWidget* mCenterWidget;

	
	

public:
	ObjectListWidget* mObjectList;
	Controller* mController;
	ConsoleWidget* mConSole;
	FileExploreWidget* mFileExplore;	
};
