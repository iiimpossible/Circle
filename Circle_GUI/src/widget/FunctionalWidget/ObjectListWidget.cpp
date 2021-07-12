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
//	popMenu->exec(QCursor::pos()); */// �˵����ֵ�λ��Ϊ��ǰ����λ��
//
//	//QMenu *popMenu = new QMenu(personInfo_List);                 //�˵������personInfo_List�����������
//	//QAction *towhite = new QAction(tr("��������"), personInfo_List);						//�Ҽ���ʾ�Ĳ˵�
//	//QAction *toblack = new QAction(tr("��������"), personInfo_List);
//
//	//connect(towhite, SIGNAL(triggered(bool)), this, SLOT(menu_white()));    //�Ҽ��˵��¼�
//	//connect(toblack, SIGNAL(triggered(bool)), this, SLOT(menu_black()));    //�Ҽ��˵��¼�
//
//	//if (personInfo_List->itemAt(mapFromGlobal(QCursor::pos())) != NULL)       //mapFromGlobalȫ��λ�õ�ӳ��
//	//{
//	//	popMenu->addAction(towhite);																				 //���˵���ǩ������ȥ��ʾ
//	//	popMenu->addAction(toblack);
//	//}
//	//popMenu->exec(QCursor::pos()); //�˵����ֵ�λ��Ϊ��ǰ����λ��
//
//	//delete popMenu;
//	//delete towhite;
//	//delete toblack;
//	
//
//}