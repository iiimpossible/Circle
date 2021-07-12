#include "ObjectListWidget.h"
#include <qlistwidget.h>
#include <qmenu.h>
#include <qaction.h>
#include "../../controller/Controller.h"
#include <ShaderViewer/src/widget/MyListWidget.h>

ObjectListWidget::ObjectListWidget(QWidget* aWidget) :
	QDockWidget(aWidget)
{
	setWindowTitle("ObjectList");
	setObjectName("ObjectList");
	mListWidget = new SceneListWidget(this);

	setWidget(mListWidget);

	connect(mListWidget, &QListWidget::itemClicked, this, &ObjectListWidget::SlotGetListWidgetClikedItem);

}

ObjectListWidget::~ObjectListWidget()
{
}

void ObjectListWidget::UpdateObjectList(const QVector<QString>& aNames)
{
	for (auto val : aNames)
	{
		SlotAddSingleItem(val);
	}	
}



void ObjectListWidget::SlotGetListWidgetClikedItem(QListWidgetItem * aItem)
{	
	emit OnListItemCliked(aItem->text());
}

void ObjectListWidget::SlotAddSingleItem(QString aObjectName)
{
	QListWidgetItem* tItem = new QListWidgetItem(mListWidget);
	tItem->setText(aObjectName);
	mListWidget->addItem(tItem);
}

void ObjectListWidget::SetObjectMaterial(const QString& aName)
{

}

void ObjectListWidget::SlotInitObjectList(const QVector<QString>& aNames)
{
	
	mListWidget->clear();
	for (auto val : aNames)
	{
		SlotAddSingleItem(val);
	}
}


//
//void ObjectListWidget::contextMenuEvent(QContextMenuEvent *)
//{
//	/*setContextMenuPolicy(Qt::ActionsContextMenu);
//	QMenu* popMenu = new QMenu(this);
//	popMenu->addAction(new QAction("Add", this));
//	popMenu->addAction(new QAction("Delete", this));
//	popMenu->addAction(new QAction("Modify", this));
//
//	popMenu->exec(QCursor::pos()); */// 菜单出现的位置为当前鼠标的位置
//
//	//QMenu *popMenu = new QMenu(personInfo_List);                 //菜单添加在personInfo_List这个对象里面
//	//QAction *towhite = new QAction(tr("到白名单"), personInfo_List);						//右键显示的菜单
//	//QAction *toblack = new QAction(tr("到黑名单"), personInfo_List);
//
//	//connect(towhite, SIGNAL(triggered(bool)), this, SLOT(menu_white()));    //右键菜单事件
//	//connect(toblack, SIGNAL(triggered(bool)), this, SLOT(menu_black()));    //右键菜单事件
//
//	//if (personInfo_List->itemAt(mapFromGlobal(QCursor::pos())) != NULL)       //mapFromGlobal全局位置的映射
//	//{
//	//	popMenu->addAction(towhite);																				 //将菜单标签加载上去显示
//	//	popMenu->addAction(toblack);
//	//}
//	//popMenu->exec(QCursor::pos()); //菜单出现的位置为当前鼠标的位置
//
//	//delete popMenu;
//	//delete towhite;
//	//delete toblack;
//	
//
//}