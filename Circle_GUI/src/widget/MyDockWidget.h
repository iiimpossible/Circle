#pragma once
#include <qdockwidget.h>
#include <qvector3d.h>
#include <qmap.h>
class QTimer;
class TransformTableWidget;
class TransformGroupWidget;
class QVBoxLayout;
class MyWidget;
class GameObject;
class QTimerEvent;
class ComponentGroupWidget;
/**
 * @brief 一个通用的DockWidget。包含一个Widget作为容器，且拥有一个垂直布局
 */
class MyDockWidget : public QDockWidget
{
	Q_OBJECT
public: 
	MyDockWidget(QWidget* aWidget = Q_NULLPTR);

	void SetWidgetName(const QString& aName = "MyDockWidget");
	void SetBackground(const QString& aColor = "background:rgb(230, 230, 230);");
	void SetBorder(const QString& aColor);
	
	void AddWidget(QWidget* aWidget);
	QWidget* GetContent();

public slots:
	//槽函数

	

private:
	QWidget* content;
	QVBoxLayout* vLayout;
};



/*===========================================================================================================*/

class ComponentGroupWidget;

class InspectorWidget:public QDockWidget
{
	Q_OBJECT
public:
	InspectorWidget(QWidget* aWidget = Q_NULLPTR);
	~InspectorWidget();
public:
	//接口
	void AddWidget(QWidget* aWidget);
	
	void SetPosInfo(const QVector3D& aPos);
	void SetRotationInfo(const QVector3D& aRot);
	void SetSizeInfo(const QVector3D& aSize);

signals:

	
	/**
	 * @brief 游戏物体的Transform信息被改变，传递该游戏物体的名字以及被改变的信息
	 * @param aObjectName 游戏物体的名字
	 * @param aVec 几何信息
	 * @param aTag 是位置1、旋转2 还是大小3？
	 */
	void OnObjectTransforModified(const QString& aObjectName, const QVector3D& aVec, int aTag); 

	void OnComponentCommandEditFinished(const QString& aObjectName, const QString& aComponentType, const QString& aCommand);
public slots:

	/** 更新当前游戏物体的信息，这个槽是被绑定到Objectlist widget*/
	void SlotUpdateWidget(QString aObjectName);



private slots:

	/** 获得用户改变的游戏物体的transform的信息,每个SpinBox的改变都会调用该函数，
	这个槽绑定到自己的spinbox*/
	void SlotGetObjectModifiedPosition(const QVector3D& aVec);
	void SlotGetObjectModifiedRotation(const QVector3D& aVec);
	void SlotGetObjectModifiedSize(const QVector3D& aVec);
	void SlotGetComponentCommand(const QString& aComponentType, const QString& aCommand);
private:
	//事件重写
	//	InspectorWidget::leaveEvent()
	virtual void timerEvent(QTimerEvent* aEvent) override;

	virtual void leaveEvent(QEvent* aEvent) override;

	//virtual void focusInEvent(QFocusEvent *event)override;

	//virtual void focusOutEvent(QFocusEvent *event)override;

	//virtual void activeEvent()override;
private:

	/** 处理定时器到点的工作，更新transform*/
	void HandleTimeOut();

	//通过给定的组件列表生成组件UI，组件UI包含一个组件名和一个指令输入窗口

	void CreateComponentWidget(const QString& aCompType);

	void DeleteComponentWidget(const QString& aCompType);

	void DeleteComponentWidget();

private:
	TransformGroupWidget* mPosinfoWidget;
	TransformGroupWidget* mRotationWidget;
	TransformGroupWidget* mSizeWidget;

	QWidget* mContent;
	QVBoxLayout* mVLayout;

	QString mCurrentObjectName;

	QTimer* mTimer;

	int mTimerID;

	 
	QMap < QString, ComponentGroupWidget* > mComponentWidgets;

};

