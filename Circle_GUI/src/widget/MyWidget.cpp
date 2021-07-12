#include <qwidget.h>
#include <qevent.h>
#include <qsize.h>
#include "MyWidget.h"

MyWidget::MyWidget(QWidget* widget):
	QWidget(widget)
{
	vLayout = new QVBoxLayout();
	vLayout->setSpacing(5);
	vLayout->setMargin(3);
	vLayout->setAlignment(Qt::AlignTop);

	this->setLayout(vLayout);

	setStyleSheet("background:rgb(95, 179, 180);");
}

void MyWidget::AddWidget(QWidget* aWidget)
{
	vLayout->addWidget(aWidget);
	aWidget->setParent(this);

}



/*==============================================================================================================*/

MyCenterWidget::MyCenterWidget(QWidget* aWidget):
	QWidget(aWidget)
{

}


MyCenterWidget::~MyCenterWidget()
{
}

void MyCenterWidget::resizeEvent(QResizeEvent * aEvent)
{
	QSize size = aEvent->size();
	OnCenterWidgetResize(size.width(),size.height());
	QWidget::resizeEvent(aEvent);
}
