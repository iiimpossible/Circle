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
	//测试
	
	//MainWidget设置
	//showFullScreen();
	//showMaximized();

	//添加控件
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
	//信号槽

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


/*==========================游戏物体列表操作=================================================================================*/
		

	//创建Object：当创建完游戏物体后，在游戏物体列表上添加一个与该游戏物体对应的项目
	connect(mController, &Controller::OnGameObjecetCreate, mObjectList, &ObjectListWidget::SlotAddSingleItem);

	//场景：加载完场景后，更新游戏物体列表的显示。TODO:删掉上一个场景的项目
	connect(mController, &Controller::OnSceneLoad, mObjectList, &ObjectListWidget::SlotInitObjectList);

	//改名：发送游戏物体改名信号
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnRenameGameObject, mController, &Controller::SlotSetObjectName);

	//改名：当游戏物体名字修改完成后，从引擎库中返回最终的名字
	connect(mController, &Controller::OnGameobjectRenamed, mObjectList->GetListWidget(), &SceneListWidget::SlotOnGameObjectRenamed);

	//改材质：给游戏物体列表的对应物体更换材质
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnChangeMaterial, mController, &Controller::SlotSetObjctMaterial);

	//更新Object信息：当物体列表项目被点击，更新Inspector面板物体位置信息
	connect(mObjectList, &ObjectListWidget::OnListItemCliked, mInspector, &InspectorWidget::SlotUpdateWidget);

	//这个没用
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnItemActionTrigger, mController, &Controller::SlotObjectListAction);

	//调试：给物体列表添加调试信息
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnDebugLog, mConSole, &ConsoleWidget::SlotAddDebugLog);

	//删除一个游戏物体
	connect(mObjectList->GetListWidget(), &SceneListWidget::OnDeleteGameObject, mController, &Controller::SlotDeleteGameObject);


/*================================文件浏览面板操作===========================================================================*/


	//当加载完材质后，在材质列表添加一个项目
	connect(mController, &Controller::OnMaterialLoad, mFileExplore, &FileExploreWidget::AddListItem);	

	//当场景加载完后，更新材质列表信息
	connect(mController, &Controller::OnSceneLoadMaterialRefresh, mFileExplore, &FileExploreWidget::SlotRefreshMaterialList);

	//对游戏物体的修改  需要获得游戏物体名字,以及信息类型tag
	connect(mInspector, &InspectorWidget::OnObjectTransforModified, mController, &Controller::SlotObjectTransformModified);

	//对游戏物体的组件进行操作
	connect(mInspector, &InspectorWidget::OnComponentCommandEditFinished, mController, &Controller::SlotSendComponentCommand);


/*=================================其他功能操作==========================================================================*/


	//改变图新窗口的大小
	connect(mCenterWidget, &MyCenterWidget::OnCenterWidgetResize, mController, &Controller::SlotResizeCurrentGraphicWindow);

	 
/*================================游戏物体工厂信息===========================================================================*/


	//从游戏物体工厂中获得注册后的游戏物体类型信息
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
 
