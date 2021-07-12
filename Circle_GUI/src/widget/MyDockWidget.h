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
 * @brief һ��ͨ�õ�DockWidget������һ��Widget��Ϊ��������ӵ��һ����ֱ����
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
	//�ۺ���

	

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
	//�ӿ�
	void AddWidget(QWidget* aWidget);
	
	void SetPosInfo(const QVector3D& aPos);
	void SetRotationInfo(const QVector3D& aRot);
	void SetSizeInfo(const QVector3D& aSize);

signals:

	
	/**
	 * @brief ��Ϸ�����Transform��Ϣ���ı䣬���ݸ���Ϸ����������Լ����ı����Ϣ
	 * @param aObjectName ��Ϸ���������
	 * @param aVec ������Ϣ
	 * @param aTag ��λ��1����ת2 ���Ǵ�С3��
	 */
	void OnObjectTransforModified(const QString& aObjectName, const QVector3D& aVec, int aTag); 

	void OnComponentCommandEditFinished(const QString& aObjectName, const QString& aComponentType, const QString& aCommand);
public slots:

	/** ���µ�ǰ��Ϸ�������Ϣ��������Ǳ��󶨵�Objectlist widget*/
	void SlotUpdateWidget(QString aObjectName);



private slots:

	/** ����û��ı����Ϸ�����transform����Ϣ,ÿ��SpinBox�ĸı䶼����øú�����
	����۰󶨵��Լ���spinbox*/
	void SlotGetObjectModifiedPosition(const QVector3D& aVec);
	void SlotGetObjectModifiedRotation(const QVector3D& aVec);
	void SlotGetObjectModifiedSize(const QVector3D& aVec);
	void SlotGetComponentCommand(const QString& aComponentType, const QString& aCommand);
private:
	//�¼���д
	//	InspectorWidget::leaveEvent()
	virtual void timerEvent(QTimerEvent* aEvent) override;

	virtual void leaveEvent(QEvent* aEvent) override;

	//virtual void focusInEvent(QFocusEvent *event)override;

	//virtual void focusOutEvent(QFocusEvent *event)override;

	//virtual void activeEvent()override;
private:

	/** ����ʱ������Ĺ���������transform*/
	void HandleTimeOut();

	//ͨ������������б��������UI�����UI����һ���������һ��ָ�����봰��

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

