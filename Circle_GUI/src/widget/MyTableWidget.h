#pragma once
#include <qtablewidget.h>
#include <QVBoxLayout>
#include <qvector3d.h>
class TransformTableWidget:
	public QTableWidget
{
	Q_OBJECT
public:
	TransformTableWidget(QWidget* tableWidget = nullptr);

	
	void SetColumnsWidth(int width);
	void SetRowsHeight(int height);	
	void SetLabelText(const QString& first = "x:",const QString& second = "y:", const QString& third = "z:");
	void SetInfoText(const QVector3D& info);
	void SetInfoText(const QString& first = "0.0", const QString& second = "0.0", const QString& third = "0.0");

	void SetTableName(const QString& name);
private:
	int columns;
	int rows;
};

#include <qlabel.h>

/**
 * @brief 继承于QWidget。这个用来显示游戏对象的一些transform信息
 * 包含一个3*2的表格，就是显示一个3维向量，并带一个名字
 */
class TransformInfoWidget :public QWidget
{
	Q_OBJECT
public:
	TransformInfoWidget(QWidget* widget = nullptr);
	

private:
	QLabel* name;
	TransformTableWidget* info;
	QVBoxLayout* vLayout;
};

