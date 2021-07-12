#include "ShaderViewer.h"
#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <qtabbar.h>
#include <qdockwidget.h>
#include <qdebug.h>
#include <qdialog.h>
#include <qfiledialog.h>
#include <qvector3d.h>
#include "src/controller/Controller.h"
//#include "src/widget/MyLabel.h"
#include "src/widget/MyTableWidget.h"
#include "src/widget/MyDockWidget.h"
#include "src/widget/FileSystemWidget.h"
#include "src/widget/MyWidget.h"
#include "src/widget/FunctionalWidget/Console.h"
#include "src/widget/FunctionalWidget/ObjectListWidget.h"
#include "src/widget/MyDialog.h"
#include "src/widget/FunctionalWidget/FileExplore.h"
#include "src/widget/MyWidget.h"
ShaderViewer::ShaderViewer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	///showMaximized();
	setWindowTitle("CircleEngine v0.1");
	//����
	
	//MainWidget����
	//showFullScreen();
	//showMaximized();

	//��ӿؼ�
	mCenterWidget = new MyCenterWidget(this);
	mCenterWidget->setStyleSheet(QString::fromUtf8("border:10px solid red"));
	this->setCentralWidget(mCenterWidget);

	mInspector = new InspectorWidget(this);

	addDockWidget(Qt::RightDockWidgetArea, mInspector);
	

	dock = new MyDockWidget(this);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
	//dock->AddWidget(new TransformTableWidget());
	
	dock->AddWidget(new FileSystemWidget(dock->GetContent()));
	dock->SetWidgetName("FileExplorer");
	dock->hide();
	//dock->setWidget(fi);
	//dock->setStyleSheet("back");

	mConSole = new ConsoleWidget(this);
	addDockWidget(Qt::BottomDockWidgetArea, mConSole);
	

	mObjectList = new ObjectListWidget(this);
	addDockWidget(Qt::LeftDockWidgetArea, mObjectList);

	mFileExplore = new FileExploreWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, mFileExplore);


	//addAction(new QAction("Create Material", this));
	//addAction(new QAction("Create Scene", this));
	//setContextMenuPolicy(Qt::ActionsContextMenu);



	//connect(ui.actionSqure, &QAction::triggered, [=](){
		//Controller::LoadMaterial("asset/myMaterial.material");
	//});


}

void ShaderViewer::ShowWidgets()
{
	mInspector->show();
	dock->show();
}


#include <ShaderViewer/src/widget/MyListWidget.h>
void ShaderViewer::PlugSocket()
{
	//�źŲ�

	connect(ui.actionMaterial, &QAction::triggered, [this]() {
		QUrl path =  QFileDialog::getOpenFileUrl(this,"Selete Material",QUrl("ShaderViewer"));
		mController->LoadMaterial(path);		
	});
	
	
	connect(ui.actionModel, &QAction::triggered, [this]() {
		QUrl path = QFileDialog::getOpenFileUrl(this, "Selete Model", QUrl("ShaderViewer"));
		mController->LoadModel(path);
	});

	connect(ui.actionScene, &QAction::triggered, [this]() {
		QUrl path = QFileDialog::getOpenFileUrl(this, "Selete Scene",QUrl("ShaderViewer"));
		mController->LoadScene(path);
	});

	connect(ui.actionSave, &QAction::triggered, [this]()
	{
		QUrl path = QFileDialog::getOpenFileUrl(this, "Selete Scene", QUrl("ShaderViewer"));
		mController->SaveScene(path);
	});

	//connect(ui.actio)
	connect(ui.actionSkybox, &QAction::triggered, [this]() {
		QUrl path = QFileDialog::getOpenFileUrl(this, "Selete Skybox", QUrl("ShaderViewer"));
		mController->LoadSkybox(path);
	});


/*==========================��Ϸ�����б����=================================================================================*/
		

	//����Object������������Ϸ���������Ϸ�����б������һ�������Ϸ�����Ӧ����Ŀ
	connect(mController, &Controller::OnGameObjecetCreate, mObjectList, &ObjectListWidget::SlotAddSingleItem);

	//�����������곡���󣬸�����Ϸ�����б����ʾ��TODO:ɾ����һ����������Ŀ
	connect(mController, &Controller::OnSceneLoad, mObjectList, &ObjectListWidget::SlotInitObjectList);

	//������������Ϸ��������ź�
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnRenameGameObject, mController, &Controller::SlotSetObjectName);

	//����������Ϸ���������޸���ɺ󣬴�������з������յ�����
	connect(mController, &Controller::OnGameobjectRenamed, mObjectList->GetListWidget(), &SceneListWidget::SlotOnGameObjectRenamed);

	//�Ĳ��ʣ�����Ϸ�����б�Ķ�Ӧ�����������
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnChangeMaterial, mController, &Controller::SlotSetObjctMaterial);

	//����Object��Ϣ���������б���Ŀ�����������Inspector�������λ����Ϣ
	connect(mObjectList, &ObjectListWidget::OnListItemCliked, mInspector, &InspectorWidget::SlotUpdateWidget);

	//���û��
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnItemActionTrigger, mController, &Controller::SlotObjectListAction);

	//���ԣ��������б���ӵ�����Ϣ
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnDebugLog, mConSole, &ConsoleWidget::SlotAddDebugLog);

	//ɾ��һ����Ϸ����
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnDeleteGameObject, mController, &Controller::SlotDeleteGameObject);


/*================================�ļ����������===========================================================================*/


	//����������ʺ��ڲ����б����һ����Ŀ
	connect(mController, &Controller::OnMaterialLoad, mFileExplore, &FileExploreWidget::AddListItem);	

	//������������󣬸��²����б���Ϣ
	connect(mController, &Controller::OnSceneLoadMaterialRefresh, mFileExplore, &FileExploreWidget::SlotRefreshMaterialList);

	//����Ϸ������޸�  ��Ҫ�����Ϸ��������,�Լ���Ϣ����tag
	connect(mInspector, &InspectorWidget::OnObjectTransforModified, mController, &Controller::SlotObjectTransformModified);

	//����Ϸ�����������в���
	connect(mInspector, &InspectorWidget::OnComponentCommandEditFinished, mController, &Controller::SlotSendComponentCommand);


/*=================================�������ܲ���==========================================================================*/


	//�ı�ͼ�´��ڵĴ�С
	connect(mCenterWidget, &MyCenterWidget::OnCenterWidgetResize, mController, &Controller::SlotResizeCurrentGraphicWindow);

	 
/*================================��Ϸ���幤����Ϣ===========================================================================*/


	//����Ϸ���幤���л��ע������Ϸ����������Ϣ
	QList<QAction*> object_types;
	for (auto type : mController->GetObjectTypes())
	{
		QAction* act = new QAction(this);
		act->setText(type);
		object_types.push_back(act);
		connect(act, &QAction::triggered, [=]() {
			this->mController->CreateObject(act->text());
		});
	}
	ui.menu3DObject->addActions(object_types);
}

void ShaderViewer::RightButtonMenu()
{
	addAction(new QAction("Create Material", this));
	addAction(new QAction("Create Scene", this));
	setContextMenuPolicy(Qt::ActionsContextMenu);
}
 
