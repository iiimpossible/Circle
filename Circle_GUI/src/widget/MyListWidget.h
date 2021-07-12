#pragma once
#include <qlistwidget.h>
#include <qvector.h>
#include <ShaderViewer/src/common/MyCommon.h>


class QAction;
class ListMenuWidget;

/**
 * @brief 自定义listwidget基类
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
 * @brief 继承于QListWidgt.一个Item显示一个场景，而场景下边包含一系列Object树
 * 点击事件：点击会选中当前场景内的物体
 * 操作：删除场景内物体
 * 显示：物体增加需要更新（多播委托：CreateObject函数触发）
 * @todo: 通用item操作信号，参数：操作枚举，Item名字
 */
class SceneListWidget:public QListWidget
{
	Q_OBJECT
public:
	SceneListWidget(QWidget* aWidget = Q_NULLPTR);
public:
	//



signals:

	/** 发送当前被选中的需要修改材质的Item的name*/
	void OnItemSetMarerial(const QString& aName);

	/** 当Item被选中时发送该Item的Text和  该函数删除*/
	void OnItemActionTrigger(EObjectListAction aAction, const QString& aObjectName, const QString& aMaterialName);

	/** 发送调试信息*/
	void OnDebugLog(const QString& aInfo);

	void OnChangeMaterial(const QString& aObjectName, const QString& aMaterialName);

	void OnDeleteGameObject(const QString& aObjectName);

	void OnRenameGameObject(const QString& aOldName, const QString& aNewName);

public slots:

	/** 当右键菜单中的setMaterial触发后，发送信号*/
	void SlotEmitOnSelectItem();

	
	
	/** @breif 当游戏物体的名字改变完成，从引擎库中返回最终的名字*/
	void SlotOnGameObjectRenamed(const QString& aNewname);

	//这个槽函数可以获得被点击的qlistwidgetitem
	//void SlotGetClickedItem(QListWidgetItem*,int);
private slots:

	void SlotOnCurrentTextChanged(const QString& aCurrentText);

	void SlotOnCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

	void SlotOnItemChanged(QListWidgetItem* aItem);

	/** @breif 发送游戏物体删除信号*/
	void SlotEmitOnDeleteGameObject();

	/** @breif 发送游戏物体改名信号*/
	void SlotEmitOnRenameGameObject();

protected:

	virtual void contextMenuEvent(QContextMenuEvent * aEvent) override;

	//重写下面两个函数来完成从该部件中拖拽出去的操作.
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;

	//重写下面三个函数来完成接受外来拖拽的操作.
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
 
private:

	//记录拖拽的起点.
	QPoint mDragPoint;
	//记录被拖拽的项.
	QListWidgetItem *mDragItem;

	QListWidgetItem* mSlectedListItem;

	//menu item
	QAction* mSetMaterialAction;
	QAction* mDeleteAction;
	QAction* mRenameAction;

	ListMenuWidget* mMenu;
	QString mCurrentItemText;

	//修改item的文本的临时变量
	QString mOldName;
	QString mModifiedItemName;

	bool bIsRenameObject;
};

/*==============================================================================================================*/

/**
 * @brief 已经编译的材质的列表
 * @detail 该列表的意义是放便地查看已经编译的材质，然后将材质从这个列表拖动
 * 到游戏物体列表的item上，给对应的游戏物体更换材质。
 * @func 也就是说不接收拖动输入，但是可以输出。mousePressEvent mouseMoveEvent
 * @func 有右键菜单，需要重载。contextMenuEvent
 * @func 调试信息信号
 * 
 */
class MaterialListWidget :public QListWidget
{
	Q_OBJECT
public:
	MaterialListWidget(QWidget* aWidget = Q_NULLPTR);
public:

signals:
	/** 发送调试信息*/
	void OnDebugLog(const QString& aInfo);

public slots:

	 

protected:
	//重写事件函数、上下文菜单

	//重写下面两个函数来完成从该部件中拖拽出去的操作.
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	//void contextMenuEvent(QContextMenuEvent * aEvent);
private:

	//记录拖拽的起点.
	QPoint mDragPoint;
	//记录被拖拽的项.
	QListWidgetItem *mDragItem;
};


/*==============================================================================================================*/


class DialogListWidget :public QListWidget
{
	Q_OBJECT
public:
	DialogListWidget(QWidget* aWidget = Q_NULLPTR);

public:

	/** 初始化这个list*/
	void InitItemList(QVector<QString>& aTexts);

signals:

	void OnItemTriggered(const QString& aText);

public slots:

	void SlotSentItemText(QListWidgetItem* aItem);
private:
	
};