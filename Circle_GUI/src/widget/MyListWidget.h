#pragma once
#include <qlistwidget.h>
#include <qvector.h>
#include <ShaderViewer/src/common/MyCommon.h>


class QAction;
class ListMenuWidget;

/**
 * @brief �Զ���listwidget����
 */
class CustomListWidget
{
public:
	CustomListWidget();
	~CustomListWidget();

private:

};

/*==============================================================================================================*/

/**
 * @brief �̳���QListWidgt.һ��Item��ʾһ���������������±߰���һϵ��Object��
 * ����¼��������ѡ�е�ǰ�����ڵ�����
 * ������ɾ������������
 * ��ʾ������������Ҫ���£��ಥί�У�CreateObject����������
 * @todo: ͨ��item�����źţ�����������ö�٣�Item����
 */
class SceneListWidget:public QListWidget
{
	Q_OBJECT
public:
	SceneListWidget(QWidget* aWidget = Q_NULLPTR);
public:
	//



signals:

	/** ���͵�ǰ��ѡ�е���Ҫ�޸Ĳ��ʵ�Item��name*/
	void OnItemSetMarerial(const QString& aName);

	/** ��Item��ѡ��ʱ���͸�Item��Text��  �ú���ɾ��*/
	void OnItemActionTrigger(EObjectListAction aAction, const QString& aObjectName, const QString& aMaterialName);

	/** ���͵�����Ϣ*/
	void OnDebugLog(const QString& aInfo);

	void OnChangeMaterial(const QString& aObjectName, const QString& aMaterialName);

	void OnDeleteGameObject(const QString& aObjectName);

	void OnRenameGameObject(const QString& aOldName, const QString& aNewName);

public slots:

	/** ���Ҽ��˵��е�setMaterial�����󣬷����ź�*/
	void SlotEmitOnSelectItem();

	
	
	/** @breif ����Ϸ��������ָı���ɣ���������з������յ�����*/
	void SlotOnGameObjectRenamed(const QString& aNewname);

	//����ۺ������Ի�ñ������qlistwidgetitem
	//void SlotGetClickedItem(QListWidgetItem*,int);
private slots:

	void SlotOnCurrentTextChanged(const QString& aCurrentText);

	void SlotOnCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

	void SlotOnItemChanged(QListWidgetItem* aItem);

	/** @breif ������Ϸ����ɾ���ź�*/
	void SlotEmitOnDeleteGameObject();

	/** @breif ������Ϸ��������ź�*/
	void SlotEmitOnRenameGameObject();

protected:

	virtual void contextMenuEvent(QContextMenuEvent * aEvent) override;

	//��д����������������ɴӸò�������ק��ȥ�Ĳ���.
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;

	//��д����������������ɽ���������ק�Ĳ���.
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
 
private:

	//��¼��ק�����.
	QPoint mDragPoint;
	//��¼����ק����.
	QListWidgetItem *mDragItem;

	QListWidgetItem* mSlectedListItem;

	//menu item
	QAction* mSetMaterialAction;
	QAction* mDeleteAction;
	QAction* mRenameAction;

	ListMenuWidget* mMenu;
	QString mCurrentItemText;

	//�޸�item���ı�����ʱ����
	QString mOldName;
	QString mModifiedItemName;

	bool bIsRenameObject;
};

/*==============================================================================================================*/

/**
 * @brief �Ѿ�����Ĳ��ʵ��б�
 * @detail ���б�������Ƿű�ز鿴�Ѿ�����Ĳ��ʣ�Ȼ�󽫲��ʴ�����б��϶�
 * ����Ϸ�����б��item�ϣ�����Ӧ����Ϸ����������ʡ�
 * @func Ҳ����˵�������϶����룬���ǿ��������mousePressEvent mouseMoveEvent
 * @func ���Ҽ��˵�����Ҫ���ء�contextMenuEvent
 * @func ������Ϣ�ź�
 * 
 */
class MaterialListWidget :public QListWidget
{
	Q_OBJECT
public:
	MaterialListWidget(QWidget* aWidget = Q_NULLPTR);
public:

signals:
	/** ���͵�����Ϣ*/
	void OnDebugLog(const QString& aInfo);

public slots:

	 

protected:
	//��д�¼������������Ĳ˵�

	//��д����������������ɴӸò�������ק��ȥ�Ĳ���.
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	//void contextMenuEvent(QContextMenuEvent * aEvent);
private:

	//��¼��ק�����.
	QPoint mDragPoint;
	//��¼����ק����.
	QListWidgetItem *mDragItem;
};


/*==============================================================================================================*/


class DialogListWidget :public QListWidget
{
	Q_OBJECT
public:
	DialogListWidget(QWidget* aWidget = Q_NULLPTR);

public:

	/** ��ʼ�����list*/
	void InitItemList(QVector<QString>& aTexts);

signals:

	void OnItemTriggered(const QString& aText);

public slots:

	void SlotSentItemText(QListWidgetItem* aItem);
private:
	
};