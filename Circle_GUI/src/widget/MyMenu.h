#pragma once

#include <qmenu.h>

/**
 * @brief 自定义的列表右键菜单
 * @detail 主要即使重写了hideEvent，当这个菜单被隐藏了之后，就会执行重写的hideEvent，
 * 清除该菜单中的Action
 */
class ListMenuWidget:public QMenu
{
	//这里不能添加这个宏，导致无法解析的外部符号，不知道为啥
	//Q_OBJECT
public:
	ListMenuWidget(QWidget* aWidget = Q_NULLPTR);
	~ListMenuWidget();

private:

	/** 重载隐藏事件，clear这个menu中的action*/
	virtual void hideEvent(QHideEvent* aEvent) override;
};


 