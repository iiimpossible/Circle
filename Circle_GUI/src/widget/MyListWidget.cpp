#include "MyListWidget.h"
#include <qmenu.h>
#include <qaction.h>
#include <qdebug.h>
#include <qlistwidget>
#include <qcolordialog.h>
#include <qdialog.h>
#include <qdrag.h>
#include <qapplication.h>
#include <qmimedata.h>
#include <qmessagebox.h>
#include <ShaderViewer/src/widget/FunctionalWidget/Console.h>
#include <ShaderViewer/src/widget/MyMenu.h>
#include <ShaderViewer/src/widget/MyDialog.h>

SceneListWidget::SceneListWidget(QWidget* aWidget):
	QListWidget(aWidget)
{
	mCurrentItemText = "";
	mMenu = new ListMenuWidget(this);
	mSetMaterialAction = new QAction("SetMaterial", this);
	mDeleteAction = new QAction("Delete", this);
	mRenameAction = new QAction("Rename", this);

	bIsRenameObject = false;

	//setMovement(QListView::Snap);
	setAcceptDrops(true);//接收拖拽
	connect(mSetMaterialAction, &QAction::triggered, this, &SceneListWidget::SlotEmitOnSelectItem);
	connect(mDeleteAction, &QAction::triggered, this, &SceneListWidget::SlotEmitOnDeleteGameObject);
	connect(mRenameAction, &QAction::triggered, this, &SceneListWidget::SlotEmitOnRenameGameObject);
	//connect(this, &SceneListWidget::itemChanged, this, &SceneListWidget::SlotOnItemChanged);
 
}

//当DeleteAction触发的时候调用
void SceneListWidget::SlotEmitOnDeleteGameObject()
{
	emit OnDeleteGameObject(mCurrentItemText);
	mCurrentItemText.clear();
}

//当RenameAction触发的时候调用
void SceneListWidget::SlotEmitOnRenameGameObject()
{
	
	QListWidgetItem* cur_item = currentItem();
	if (!cur_item) return;
	mOldName = cur_item->text();
	cur_item->setFlags(cur_item->flags() | Qt::ItemIsEditable);
	
	QListWidget::editItem(cur_item);//这里修改item的文本，修改完毕后会触发itemchanged信号t
	
	//发送信号，修改游戏物体的名字
	//emit OnRenameGameObject(mModifiedItemName, mSlectedListItem->text());
	connect(this, &SceneListWidget::itemChanged, this, &SceneListWidget::SlotOnItemChanged);	
}

void SceneListWidget::SlotOnItemChanged(QListWidgetItem* aItem)
{	
	auto item = currentItem();
	if (!item) return;
	 
	QString old_name = mOldName;
	QString new_name = item->text();
	if (old_name != new_name)
	{
		emit OnRenameGameObject(old_name, new_name);		
	}
	disconnect(this, &SceneListWidget::itemChanged, this, &SceneListWidget::SlotOnItemChanged);

}

void SceneListWidget::SlotOnCurrentTextChanged(const QString & aCurrentText)
{
	//emit OnRenameGameObject(mModifiedItemName, aCurrentText);
}

void SceneListWidget::SlotOnCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
	if(current)
		emit OnRenameGameObject(mModifiedItemName, current->text());
}

void SceneListWidget::SlotOnGameObjectRenamed(const QString & aNewname)
{
	QListWidgetItem* cur_item = currentItem();
	if (!cur_item)return;
	if (cur_item->text() != aNewname)
	{
		cur_item->setText(aNewname);
	}
}

void SceneListWidget::contextMenuEvent(QContextMenuEvent * aEvent)
{	
	//setContextMenuPolicy(Qt::CustomContextMenu);
	mSlectedListItem = itemAt(mapFromGlobal(QCursor::pos()));
	if (mSlectedListItem != NULL) //如果有item则添加"修改"菜单 [1]*
	{
		mCurrentItemText = mSlectedListItem->text();
		mMenu->addAction(mSetMaterialAction);
		mMenu->addAction(mDeleteAction);		
		mMenu->addAction(mRenameAction);
	}
	mMenu->popup(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}



void SceneListWidget::SlotEmitOnSelectItem()
{

	//应该是弹出一个列表对话框，这个对话框列表包含当前已经编译后的材质的着色器


	ListWidgetDialog* dia = new ListWidgetDialog(this);

	dia->show();
	dia->setModal(false);

	QColorDialog* col = new QColorDialog(this);

	col->show();
	//选中目标材质，获得其名字


	//如果不为空发送信号
	//if (!mCurrentItemText.isEmpty())
	//	OnItemSetMarerial(mCurrentItemText);	

}


//重写鼠标点击操作.
void SceneListWidget::mousePressEvent(QMouseEvent *event)
{
	//确保左键拖拽.
	if (event->button() == Qt::LeftButton)
	{
		//先保存拖拽的起点.
		mDragPoint = event->pos();
		//保留被拖拽的项.
		//emit OnDebugLog("@MyListWidget LOG: Get seleted item to drag.");
		mDragItem = this->itemAt(event->pos());
		//emit OnDebugLog("click" + QString::number((int)mDragItem, 16));
	}
	//保留原QListWidget部件的鼠标点击操作.
	QListWidget::mousePressEvent(event);

}
void SceneListWidget::mouseMoveEvent(QMouseEvent *event)
{
	//确保按住左键移动.
	if (!mDragItem)
	{
		//emit OnDebugLog("drag invalid"+QString::number((int)mDragItem,16));
		QListWidget::mouseMoveEvent(event);
		return;
	}
	//emit OnDebugLog("drag valid"+ QString::number((int)mDragItem, 16));
	//TODO: 需要判断控件是否在item上，目前是只要拖动就触发、阻塞
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint temp = event->pos() - mDragPoint;
		//只有这个长度大于默认的距离,才会被系统认为是形成拖拽的操作.
		if (temp.manhattanLength() > QApplication::startDragDistance())
		{
			QDrag *drag = new QDrag(this);//定义一个拖拽事件
			QMimeData *mimeData = new QMimeData;
			mimeData->setText(mDragItem->text());//这个是将mDragIntem的文字复制给这个QMinedata
			drag->setMimeData(mimeData);
			Qt::DropAction action = drag->exec(Qt::CopyAction | Qt::MoveAction);//启动拖拽事件,这里会阻塞事件循环	
			if (action == (Qt::CopyAction) || (action == Qt::MoveAction))//这个是返回值判断			
			{
				if (this->geometry().contains(mapFromGlobal(QCursor::pos())))
				{
					//当成功拖拽后，删除拖拽项.  
					//emit OnDebugLog("@MyListWidget Delete: Release mDragItem.");
					int r = row(mDragItem);
					auto item = this->takeItem(r);
					delete item;
					item = 0;
				}			
			}
		}
	}
	QListWidget::mouseMoveEvent(event);

}

void SceneListWidget::dragEnterEvent(QDragEnterEvent *event)
{
	//设置动作为移动动作.
	event->setDropAction(Qt::MoveAction);
	//然后接受事件.这个一定要写.
	event->accept();
}
void SceneListWidget::dragMoveEvent(QDragMoveEvent *event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

//当拖拽项被放下时的操作.
void SceneListWidget::dropEvent(QDropEvent *event)
{

	//这一段要重写，只是材质列表将文字输入到游戏物体列表
	QString str = event->mimeData()->text();
	if (!str.isEmpty())
	{
		//找到当前鼠标位置在部件中的项.
		auto item = this->itemAt(event->pos());
		//
		//if (item)
			//this->addItem(str);
		//else
			//this->insertItem(this->row(item), str);

		//将材质赋值给这个item对应的游戏物体
		if (item)
		{
			emit OnChangeMaterial(item->text(),str);
		}

		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	
}


/*==============================================================================================================*/

MaterialListWidget::MaterialListWidget(QWidget* aWidget):
	QListWidget(aWidget)
{

}



//重写鼠标点击操作.
void MaterialListWidget::mousePressEvent(QMouseEvent *event)
{
	//确保左键拖拽.
	if (event->button() == Qt::LeftButton)
	{
		//先保存拖拽的起点.
		mDragPoint = event->pos();
		//保留被拖拽的项.
		emit OnDebugLog("@MaterialListWidget LOG: Get seleted item to drag.");
		mDragItem = this->itemAt(event->pos());
	}
	//保留原QListWidget部件的鼠标点击操作.
	QListWidget::mousePressEvent(event);

}
void MaterialListWidget::mouseMoveEvent(QMouseEvent *event)
{
	//确保按住左键移动.
	if (!mDragItem)
	{
		emit OnDebugLog("@MaterialListWidget ERROR: mDragItem invalid.");
		return;
	}

	//TODO: 需要判断控件是否在item上，目前是只要拖动就触发、阻塞
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint temp = event->pos() - mDragPoint;
		//只有这个长度大于默认的距离,才会被系统认为是形成拖拽的操作.
		if (temp.manhattanLength() > QApplication::startDragDistance())
		{
			QDrag *drag = new QDrag(this);//定义一个拖拽事件
			QMimeData *mimeData = new QMimeData;
			mimeData->setText(mDragItem->text());//这个是将mDragIntem的文字复制给这个QMinedata
			drag->setMimeData(mimeData);
			Qt::DropAction action = drag->exec(Qt::CopyAction | Qt::MoveAction);//启动拖拽事件,这里会阻塞事件循环	
			if (action == (Qt::CopyAction) || (action == Qt::MoveAction))//这个是返回值判断			
			{
				if (this->geometry().contains(mapFromGlobal(QCursor::pos())))
				{
					//当成功拖拽后，删除拖拽项.  
					emit OnDebugLog("@MaterialListWidget Delete: Release mDragItem.");
					int r = row(mDragItem);
					auto item = this->takeItem(r);
					delete item;
					item = 0;
				}
			}
		}
	}
	QListWidget::mouseMoveEvent(event);

}



/*======================================================================================================================*/



DialogListWidget::DialogListWidget(QWidget* aWidget):
	QListWidget(aWidget)
{
	//resize(100,100);

	connect(this, &DialogListWidget::itemClicked, this, &DialogListWidget::SlotSentItemText);
}

void DialogListWidget::InitItemList(QVector<QString>& aTexts)
{
	for (auto val : aTexts)
	{
		QListWidgetItem* item = new QListWidgetItem(this);
		item->setText(val);
		addItem(item);
	}
	
}


void DialogListWidget::SlotSentItemText(QListWidgetItem* aItem)
{	
	if (aItem != NULL) //如果有item发送信号
	{
		emit OnItemTriggered(aItem->text());
	}
	
}