#pragma once

#include <qmenu.h>

/**
 * @brief �Զ�����б��Ҽ��˵�
 * @detail ��Ҫ��ʹ��д��hideEvent��������˵���������֮�󣬾ͻ�ִ����д��hideEvent��
 * ����ò˵��е�Action
 */
class ListMenuWidget:public QMenu
{
	//���ﲻ���������꣬�����޷��������ⲿ���ţ���֪��Ϊɶ
	//Q_OBJECT
public:
	ListMenuWidget(QWidget* aWidget = Q_NULLPTR);
	~ListMenuWidget();

private:

	/** ���������¼���clear���menu�е�action*/
	virtual void hideEvent(QHideEvent* aEvent) override;
};


 