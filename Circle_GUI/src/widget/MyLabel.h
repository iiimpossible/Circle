#pragma once
#include <qgroupbox.h>

class QLineEdit;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class QVector3D;
class QDoubleSpinBox;
class QLabel;
/**
 * @brief 这个是一个自定义的用于显示Transform信息的widget
 * 由标签名，和后边的信息组成，如Postion x  y  z
 */
class TransformGroupWidget:public QGroupBox
{
	Q_OBJECT
public:

	TransformGroupWidget(QWidget* label = Q_NULLPTR);
	~TransformGroupWidget();
signals:

	/** 将三个spinebox中的值都传递出去*/
	void OnValueEditFinish(const QVector3D& aInfo);


private slots:

	/** 只要三个spinebox中有一个改了，就会在这个槽触发OnValueChange，将三个boxx中的值都传递*/
	void SlotValueChange();

public:

	void SetInfoText(const QVector3D& aInfo);	
	void SetLabelText(const QString& first = "x:", const QString& second = "y:", const QString& third = "z:");
	
private:

	QVBoxLayout* mVLayout;
	QHBoxLayout* mHLayout_1;
	QHBoxLayout* mHLayout_2;
	QHBoxLayout* mHLayout_3;
	
	QLineEdit* xLabel;
	QLineEdit* yLabel;
	QLineEdit* zLabel;

	QDoubleSpinBox* xInfoLabel;
	QDoubleSpinBox* yInfoLabel;
	QDoubleSpinBox* zInfoLabel;
};


/*===========================================================================================================*/

/**
 * @breif 组件组合组件
 * @detail 当命令编辑完毕，会发送信号给inspector
 */
class ComponentGroupWidget :public QGroupBox
{
	Q_OBJECT
public:
	ComponentGroupWidget(QWidget* aWidget = Q_NULLPTR);
	~ComponentGroupWidget();

signals:

	void OnValueEditFinish(const QString& aComponentType, const QString& aCommand);
public slots:


	void SlotValueEditFinish();
private:

	//垂直布局
	QVBoxLayout* mVLayout;
	//水平布局，用来创建命令输入窗口
	QHBoxLayout* mHLayout;

	//命令输入窗口
	QLabel* mCommandLable;
	QLineEdit* mCommandInputLable;
};