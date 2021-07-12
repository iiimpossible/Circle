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
 * @brief �̳���QWidget�����������ʾ��Ϸ�����һЩtransform��Ϣ
 * ����һ��3*2�ı�񣬾�����ʾһ��3ά����������һ������
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

