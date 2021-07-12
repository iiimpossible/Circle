#include"MyLabel.h"

#include <qvector3d.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qlabel.h>


TransformGroupWidget::TransformGroupWidget(QWidget* label):
	QGroupBox(label)
{
	//QLineEdit
	//this->adjustSize();
	
	mVLayout = new QVBoxLayout(this);
	mHLayout_1 = new QHBoxLayout(this);
	mHLayout_2 = new QHBoxLayout(this);
	mHLayout_3 = new QHBoxLayout(this);

	xLabel = new QLineEdit("x:", this); xLabel->setReadOnly(true); xLabel->setMaximumWidth(30);
	yLabel = new QLineEdit("y:", this); yLabel->setReadOnly(true); yLabel->setMaximumWidth(30);
	zLabel = new QLineEdit("z:", this); zLabel->setReadOnly(true); zLabel->setMaximumWidth(30);
	
	xInfoLabel = new QDoubleSpinBox(this); xInfoLabel->setRange(-10000, 10000); xInfoLabel->setDecimals(4);
	yInfoLabel = new QDoubleSpinBox(this); yInfoLabel->setRange(-10000, 10000); yInfoLabel->setDecimals(4);
	zInfoLabel = new QDoubleSpinBox(this); zInfoLabel->setRange(-10000, 10000); zInfoLabel->setDecimals(4);

	mHLayout_1->addWidget(xLabel);
	mHLayout_1->addWidget(xInfoLabel);

	mHLayout_2->addWidget(yLabel);
	mHLayout_2->addWidget(yInfoLabel);

	mHLayout_3->addWidget(zLabel);
	mHLayout_3->addWidget(zInfoLabel);

	mVLayout->addLayout(mHLayout_1);
	mVLayout->addLayout(mHLayout_2);
	mVLayout->addLayout(mHLayout_3);

	this->setLayout(mVLayout);
	
	mVLayout->setSpacing(1);

	//void(QDoubleSpinBox::*ptr)(double) = &QDoubleSpinBox::valueChanged;
	connect(xInfoLabel, &QDoubleSpinBox::editingFinished, this, &TransformGroupWidget::SlotValueChange);
	connect(yInfoLabel, &QDoubleSpinBox::editingFinished, this, &TransformGroupWidget::SlotValueChange);
	connect(zInfoLabel, &QDoubleSpinBox::editingFinished, this, &TransformGroupWidget::SlotValueChange);
}

TransformGroupWidget::~TransformGroupWidget()
{
}

void TransformGroupWidget::SetInfoText(const QVector3D & aInfo)
{
	xInfoLabel->setValue(aInfo.x());
	yInfoLabel->setValue(aInfo.y());
	zInfoLabel->setValue(aInfo.z());
}

void TransformGroupWidget::SetLabelText(const QString & first, const QString & second, const QString & third)
{
	xLabel->setText(first);
	yLabel->setText(second);
	zLabel->setText(third);
}

void TransformGroupWidget::SlotValueChange()
{
	QVector3D vec;
	vec.setX(xInfoLabel->value());
	vec.setY(yInfoLabel->value());
	vec.setZ(zInfoLabel->value());	
	emit OnValueEditFinish(vec);	
}



/*===========================================================================================================*/



ComponentGroupWidget::ComponentGroupWidget(QWidget* aWidget)
{
	mVLayout = new QVBoxLayout(this);
	mHLayout = new QHBoxLayout(this);

	mCommandInputLable = new QLineEdit(this);
	mCommandLable = new QLabel("Command",this); mCommandLable->setMaximumWidth(60);

	mHLayout->addWidget(mCommandLable);
	mHLayout->addWidget(mCommandInputLable);

	mVLayout->addLayout(mHLayout);

	this->setLayout(mVLayout);

	connect(mCommandInputLable, &QLineEdit::editingFinished, this, &ComponentGroupWidget::SlotValueEditFinish);
}

ComponentGroupWidget::~ComponentGroupWidget()
{
}


void ComponentGroupWidget::SlotValueEditFinish()
{
	emit OnValueEditFinish(title(), mCommandInputLable->text());
}