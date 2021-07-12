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
	setAcceptDrops(true);//������ק
	connect(mSetMaterialAction, &QAction::triggered, this, &SceneListWidget::SlotEmitOnSelectItem);
	connect(mDeleteAction, &QAction::triggered, this, &SceneListWidget::SlotEmitOnDeleteGameObject);
	connect(mRenameAction, &QAction::triggered, this, &SceneListWidget::SlotEmitOnRenameGameObject);
	//connect(this, &SceneListWidget::itemChanged, this, &SceneListWidget::SlotOnItemChanged);
 
}

//��DeleteAction������ʱ�����
void SceneListWidget::SlotEmitOnDeleteGameObject()
{
	emit OnDeleteGameObject(mCurrentItemText);
	mCurrentItemText.clear();
}

//��RenameAction������ʱ�����
void SceneListWidget::SlotEmitOnRenameGameObject()
{
	
	QListWidgetItem* cur_item = currentItem();
	if (!cur_item) return;
	mOldName = cur_item->text();
	cur_item->setFlags(cur_item->flags() | Qt::ItemIsEditable);
	
	QListWidget::editItem(cur_item);//�����޸�item���ı����޸���Ϻ�ᴥ��itemchanged�ź�t
	
	//�����źţ��޸���Ϸ���������
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
	if (mSlectedListItem != NULL) //�����item�����"�޸�"�˵� [1]*
	{
		mCurrentItemText = mSlectedListItem->text();
		mMenu->addAction(mSetMaterialAction);
		mMenu->addAction(mDeleteAction);		
		mMenu->addAction(mRenameAction);
	}
	mMenu->popup(QCursor::pos()); // �˵����ֵ�λ��Ϊ��ǰ����λ��
}



void SceneListWidget::SlotEmitOnSelectItem()
{

	//Ӧ���ǵ���һ���б�Ի�������Ի����б������ǰ�Ѿ������Ĳ��ʵ���ɫ��


	ListWidgetDialog* dia = new ListWidgetDialog(this);

	dia->show();
	dia->setModal(false);

	QColorDialog* col = new QColorDialog(this);

	col->show();
	//ѡ��Ŀ����ʣ����������


	//�����Ϊ�շ����ź�
	//if (!mCurrentItemText.isEmpty())
	//	OnItemSetMarerial(mCurrentItemText);	

}


//��д���������.
void SceneListWidget::mousePressEvent(QMouseEvent *event)
{
	//ȷ�������ק.
	if (event->button() == Qt::LeftButton)
	{
		//�ȱ�����ק�����.
		mDragPoint = event->pos();
		//��������ק����.
		//emit OnDebugLog("@MyListWidget LOG: Get seleted item to drag.");
		mDragItem = this->itemAt(event->pos());
		//emit OnDebugLog("click" + QString::number((int)mDragItem, 16));
	}
	//����ԭQListWidget���������������.
	QListWidget::mousePressEvent(event);

}
void SceneListWidget::mouseMoveEvent(QMouseEvent *event)
{
	//ȷ����ס����ƶ�.
	if (!mDragItem)
	{
		//emit OnDebugLog("drag invalid"+QString::number((int)mDragItem,16));
		QListWidget::mouseMoveEvent(event);
		return;
	}
	//emit OnDebugLog("drag valid"+ QString::number((int)mDragItem, 16));
	//TODO: ��Ҫ�жϿؼ��Ƿ���item�ϣ�Ŀǰ��ֻҪ�϶��ʹ���������
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint temp = event->pos() - mDragPoint;
		//ֻ��������ȴ���Ĭ�ϵľ���,�Żᱻϵͳ��Ϊ���γ���ק�Ĳ���.
		if (temp.manhattanLength() > QApplication::startDragDistance())
		{
			QDrag *drag = new QDrag(this);//����һ����ק�¼�
			QMimeData *mimeData = new QMimeData;
			mimeData->setText(mDragItem->text());//����ǽ�mDragIntem�����ָ��Ƹ����QMinedata
			drag->setMimeData(mimeData);
			Qt::DropAction action = drag->exec(Qt::CopyAction | Qt::MoveAction);//������ק�¼�,����������¼�ѭ��	
			if (action == (Qt::CopyAction) || (action == Qt::MoveAction))//����Ƿ���ֵ�ж�			
			{
				if (this->geometry().contains(mapFromGlobal(QCursor::pos())))
				{
					//���ɹ���ק��ɾ����ק��.  
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
	//���ö���Ϊ�ƶ�����.
	event->setDropAction(Qt::MoveAction);
	//Ȼ������¼�.���һ��Ҫд.
	event->accept();
}
void SceneListWidget::dragMoveEvent(QDragMoveEvent *event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

//����ק�����ʱ�Ĳ���.
void SceneListWidget::dropEvent(QDropEvent *event)
{

	//��һ��Ҫ��д��ֻ�ǲ����б��������뵽��Ϸ�����б�
	QString str = event->mimeData()->text();
	if (!str.isEmpty())
	{
		//�ҵ���ǰ���λ���ڲ����е���.
		auto item = this->itemAt(event->pos());
		//
		//if (item)
			//this->addItem(str);
		//else
			//this->insertItem(this->row(item), str);

		//�����ʸ�ֵ�����item��Ӧ����Ϸ����
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



//��д���������.
void MaterialListWidget::mousePressEvent(QMouseEvent *event)
{
	//ȷ�������ק.
	if (event->button() == Qt::LeftButton)
	{
		//�ȱ�����ק�����.
		mDragPoint = event->pos();
		//��������ק����.
		emit OnDebugLog("@MaterialListWidget LOG: Get seleted item to drag.");
		mDragItem = this->itemAt(event->pos());
	}
	//����ԭQListWidget���������������.
	QListWidget::mousePressEvent(event);

}
void MaterialListWidget::mouseMoveEvent(QMouseEvent *event)
{
	//ȷ����ס����ƶ�.
	if (!mDragItem)
	{
		emit OnDebugLog("@MaterialListWidget ERROR: mDragItem invalid.");
		return;
	}

	//TODO: ��Ҫ�жϿؼ��Ƿ���item�ϣ�Ŀǰ��ֻҪ�϶��ʹ���������
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint temp = event->pos() - mDragPoint;
		//ֻ��������ȴ���Ĭ�ϵľ���,�Żᱻϵͳ��Ϊ���γ���ק�Ĳ���.
		if (temp.manhattanLength() > QApplication::startDragDistance())
		{
			QDrag *drag = new QDrag(this);//����һ����ק�¼�
			QMimeData *mimeData = new QMimeData;
			mimeData->setText(mDragItem->text());//����ǽ�mDragIntem�����ָ��Ƹ����QMinedata
			drag->setMimeData(mimeData);
			Qt::DropAction action = drag->exec(Qt::CopyAction | Qt::MoveAction);//������ק�¼�,����������¼�ѭ��	
			if (action == (Qt::CopyAction) || (action == Qt::MoveAction))//����Ƿ���ֵ�ж�			
			{
				if (this->geometry().contains(mapFromGlobal(QCursor::pos())))
				{
					//���ɹ���ק��ɾ����ק��.  
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
	if (aItem != NULL) //�����item�����ź�
	{
		emit OnItemTriggered(aItem->text());
	}
	
}