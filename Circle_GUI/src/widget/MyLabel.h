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
 * @brief �����һ���Զ����������ʾTransform��Ϣ��widget
 * �ɱ�ǩ�����ͺ�ߵ���Ϣ��ɣ���Postion x  y  z
 */
class TransformGroupWidget:public QGroupBox
{
	Q_OBJECT
public:

	TransformGroupWidget(QWidget* label = Q_NULLPTR);
	~TransformGroupWidget();
signals:

	/** ������spinebox�е�ֵ�����ݳ�ȥ*/
	void OnValueEditFinish(const QVector3D& aInfo);


private slots:

	/** ֻҪ����spinebox����һ�����ˣ��ͻ�������۴���OnValueChange��������boxx�е�ֵ������*/
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
 * @breif ���������
 * @detail ������༭��ϣ��ᷢ���źŸ�inspector
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

	//��ֱ����
	QVBoxLayout* mVLayout;
	//ˮƽ���֣����������������봰��
	QHBoxLayout* mHLayout;

	//�������봰��
	QLabel* mCommandLable;
	QLineEdit* mCommandInputLable;
};