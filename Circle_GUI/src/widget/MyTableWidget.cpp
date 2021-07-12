#include "MyTableWidget.h"
#include <qheaderview.h>
#include <qdebug.h>
TransformTableWidget::TransformTableWidget(QWidget* tableWidget) :
	QTableWidget(tableWidget)
{
	//清空数据
	clear();
	setShowGrid(true);

	columns = 2;
	rows = 3;
	this->setColumnCount(columns);
	this->setRowCount(rows);	

	//this->adjustSize();
	SetColumnsWidth(15);
	SetRowsHeight(10);
	
	//this->horizontalHeader()->hide();
	//horizontalHeader()->resizeSection(0,);
	//horizontalHeader()->model();

	//设置水平表头
	setHorizontalHeaderLabels(QStringList() << "Name"<<"" );
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//horizontalHeader()->hideSection(1);
	horizontalHeader()->sizeAdjustPolicy();
	horizontalHeader()->setStyleSheet("border: 1px solid black");
	//设置垂直表头
	this->verticalHeader()->hide();

	//设置滚动条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//设置自适应策略
	this->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
	setFixedHeight(107);
	setMaximumWidth(200);

	//内容初始化
	SetLabelText();
	SetInfoText();

}


void TransformTableWidget::SetColumnsWidth(int width)
{
	for (int i = 0; i < columns; i++)
	{
		this->setColumnWidth(i, width);
	}
	
}

void TransformTableWidget::SetRowsHeight(int height)
{
	for (int i = 0; i < rows; i++)
	{
		this->setRowHeight(i, height);
	}
}


void TransformTableWidget::SetTableName(const QString& name)
{
	setHorizontalHeaderLabels(QStringList() << name << "");
}


void TransformTableWidget::SetLabelText(const QString& first, const QString& second, const QString& third)
{	
	setItem(0, 0, new QTableWidgetItem(first));	
	setItem(1, 0, new QTableWidgetItem(second));
	setItem(2, 0, new QTableWidgetItem(third));
}

void TransformTableWidget::SetInfoText(const QVector3D& info)
{
	setItem(0, 1, new QTableWidgetItem(QString("%1").arg(info.x())));
	setItem(1, 1, new QTableWidgetItem(QString("%1").arg(info.y())));
	setItem(2, 1, new QTableWidgetItem(QString("%1").arg(info.z())));
}

void TransformTableWidget::SetInfoText(const QString& first, const QString& second, const QString& third)
{
	setItem(0, 1, new QTableWidgetItem(first));
	setItem(1, 1, new QTableWidgetItem(second));
	setItem(2, 1, new QTableWidgetItem(third));
}

TransformInfoWidget::TransformInfoWidget(QWidget* widget):
	QWidget(widget)
{
	
	this->name = new QLabel("Name",this);
	
	//name->show();

	info = new TransformTableWidget();
	info->setParent(this);
	//info->move(10, 50);

	vLayout = new QVBoxLayout();	

	vLayout->addWidget(name);
	vLayout->addWidget(info);
	vLayout->setSizeConstraint(QLayout::SizeConstraint::SetNoConstraint);

	this->setLayout(vLayout);

}