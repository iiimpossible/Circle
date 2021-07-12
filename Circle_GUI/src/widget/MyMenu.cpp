#include "MyMenu.h"

ListMenuWidget::ListMenuWidget(QWidget * aWidget):
	QMenu(aWidget)
{
}

ListMenuWidget::~ListMenuWidget()
{
}
void ListMenuWidget::hideEvent(QHideEvent* aEvent) 
{
	clear();
}
