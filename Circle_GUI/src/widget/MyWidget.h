#pragma once
#include <QVBoxLayout>
#include <qwidget.h>


class QResizeEvent;

/**
 * @brief ��Widgetӵ��һ����ֱ����
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
	//�ӿ�

public:
	//�¼���д

	virtual void resizeEvent(QResizeEvent * aEvent)	override;

signals:
	//�ź�
	void OnCenterWidgetResize(int aWidth, int aHeight);

public slots:

};