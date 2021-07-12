#pragma once
#include <QVBoxLayout>
#include <qwidget.h>


class QResizeEvent;

/**
 * @brief 该Widget拥有一个垂直布局
 */
class MyWidget :public QWidget
{
	Q_OBJECT
public:
	MyWidget(QWidget* aWidget = Q_NULLPTR);


	void AddWidget(QWidget* aWidget);
private:
	QVBoxLayout* vLayout;
};



class MyCenterWidget :public QWidget
{
	Q_OBJECT
public:
	MyCenterWidget(QWidget* aWidget = Q_NULLPTR);
	~MyCenterWidget();

public:
	//接口

public:
	//事件重写

	virtual void resizeEvent(QResizeEvent * aEvent)	override;

signals:
	//信号
	void OnCenterWidgetResize(int aWidth, int aHeight);

public slots:

};