#include "MyTableWidget.h"
#include <QVBoxLayout>
#include <qdebug.h>
#include <qtimer.h>
#include <qevent.h>


#include <ShaderViewer/src/controller/Controller.h>
#include "MyDockWidget.h"
#include "MyWidget.h"
#include "MyLabel.h"


MyDockWidget::MyDockWidget(QWidget* widget):
	QDockWidget(widget)
{
	
	//容器和自身配置
	content = new QWidget(this);
	

	
	content->adjustSize();
	this->content->setObjectName("content");

	this->setStyleSheet("background:rgb(210,210,210);");
	this->content->setStyleSheet("background:rgb(210,210,210);");
	SetWidgetName();
	//this->setWindowTitle("MyDockWidget");
	//this->setObjectName("MyDockWidget");

	//SetBackground();
	//布局配置
	vLayout = new QVBoxLayout();
	vLayout->setSpacing(5);
	vLayout->setMargin(3);
	vLayout->setAlignment(Qt::AlignTop);
	content->setLayout(vLayout);

	//尺寸配置
	this->setWidget(this->content);
	this->setMinimumHeight(100);
	this->setMinimumWidth(300);
	this->setMaximumWidth(400);
}

void MyDockWidget::SetWidgetName(const QString& name)
{
	this->setWindowTitle(name);
	this->setObjectName(name);
}

void MyDockWidget::SetBackground(const QString& color)
{
	this->setStyleSheet(color);
	if (content)
	{
		content->setStyleSheet(color);
	}
	
}


QWidget* MyDockWidget::GetContent()
{
	return content;
}


void MyDockWidget::AddWidget(QWidget* widget)
{
	vLayout->addWidget(widget);
}








InspectorWidget::InspectorWidget(QWidget* widget):
	QDockWidget(widget)
{

	//注意必须给widget定义一个名字才能通过stylesheet修改其外观
	this->setWindowTitle("Inspector");
	this->setObjectName("Inspector");
	
	mContent = new QWidget(this);
	mContent->adjustSize();
	this->mContent->setObjectName("content");

	//this->setStyleSheet(" #Inspector {background:rgb(95,179,180);}");
	//this->content->setStyleSheet("#content {background:rgb(95,179,180);}");
	this->setStyleSheet("background:rgb(210,210,210);");
	this->mContent->setStyleSheet("background:rgb(210,210,210);");

	mPosinfoWidget = new TransformGroupWidget(mContent); mPosinfoWidget->setTitle("Position");
	mRotationWidget = new TransformGroupWidget(mContent); mRotationWidget->setTitle("Rotation");
	mSizeWidget = new TransformGroupWidget(mContent); mSizeWidget->setTitle("Size");
	
	mVLayout = new QVBoxLayout();	
	mVLayout->addWidget(mPosinfoWidget);
	mVLayout->addWidget(mRotationWidget);
	mVLayout->addWidget(mSizeWidget);
	mVLayout->setSpacing(3);
	mVLayout->setMargin(15);
	mVLayout->setAlignment(Qt::AlignTop);	

	mTimer = new QTimer(this); mTimer->start(200);
	//qDebug()<< "layout margin is:  "<<la->margin();
	//qDebug() << "layout spacing is:  " << la->spacing();	

	mContent->setLayout(mVLayout);

	this->setWidget(this->mContent);
	this->setMinimumHeight(100);
	this->setMinimumWidth(300);
	this->setMaximumWidth(400);

	connect(mPosinfoWidget, &TransformGroupWidget::OnValueEditFinish, this, &InspectorWidget::SlotGetObjectModifiedPosition);
	connect(mRotationWidget, &TransformGroupWidget::OnValueEditFinish, this, &InspectorWidget::SlotGetObjectModifiedRotation);
	connect(mSizeWidget, &TransformGroupWidget::OnValueEditFinish, this, &InspectorWidget::SlotGetObjectModifiedSize);
	//connect(mTimer, &QTimer::timeout, this, &InspectorWidget::HandleTimeOut);

	
	
	//KillTimer();
	
}




InspectorWidget::~InspectorWidget()
{
}

void InspectorWidget::AddWidget(QWidget* widget)
{
	mVLayout->addWidget(widget);
	widget->setParent(mContent);
}

void InspectorWidget::SetPosInfo(const QVector3D& pos)
{
	mPosinfoWidget->SetInfoText(pos);
}

void InspectorWidget::SetRotationInfo(const QVector3D& rot)
{
	mRotationWidget->SetInfoText(rot);
}

void InspectorWidget::SetSizeInfo(const QVector3D& size)
{
	mSizeWidget->SetInfoText(size);
}

void InspectorWidget::SlotGetObjectModifiedPosition(const QVector3D& aVec)
{
	emit OnObjectTransforModified(mCurrentObjectName, aVec,1);
}

void InspectorWidget::SlotGetObjectModifiedRotation(const QVector3D & aVec)
{
	emit OnObjectTransforModified(mCurrentObjectName, aVec,2);
}

void InspectorWidget::SlotGetObjectModifiedSize(const QVector3D & aVec)
{
	emit OnObjectTransforModified(mCurrentObjectName, aVec,3);
}

void InspectorWidget::SlotGetComponentCommand(const QString& aComponentType,const QString & aCommand)
{
	emit OnComponentCommandEditFinished(mCurrentObjectName, aComponentType, aCommand);
}

void InspectorWidget::timerEvent(QTimerEvent * aEvent)
{	
	/*if (aEvent->timerId() == mTimerID)
	{
		HandleTimeOut();
	}*/
	QDockWidget::timerEvent(aEvent);
}

void InspectorWidget::leaveEvent(QEvent * aEvent)
{	
	/*if (mTimerID)
	{
		killTimer(mTimerID);
		mTimerID = 0;
		mCurrentObjectName = "";
	}	*/
	//qDebug() << "leave event." << endl;
	QDockWidget::leaveEvent(aEvent);
}

//void InspectorWidget::focusInEvent(QFocusEvent * event)
//{
//	qDebug() << "focus in." << endl;
//	QDockWidget::focusInEvent(event);
//}
//
//void InspectorWidget::focusOutEvent(QFocusEvent * event)
//{
//	if (mTimerID)
//	{
//		killTimer(mTimerID);
//		mTimerID = 0;
//		mCurrentObjectName = "";
//	}
//	qDebug() << "focus out." << endl;
//	QDockWidget::focusOutEvent(event);
//}

void InspectorWidget::HandleTimeOut()
{
	if (!mCurrentObjectName.isEmpty())
	{
		qDebug() << mCurrentObjectName << endl;
		auto tVal = Controller::GetGameObjectGTInfo(mCurrentObjectName);
		this->SetPosInfo(tVal.Pos);
		this->SetRotationInfo(tVal.Rot);
		this->SetSizeInfo(tVal.Size);
	}
}


	

void InspectorWidget::SlotUpdateWidget(QString aObjectName)
{
	
	//SetFocus();
	this->setFocus();
	//this->activateWindow();
	auto transfrom = Controller::GetGameObjectGTInfo(aObjectName);

	this->SetPosInfo(transfrom.Pos);
	this->SetRotationInfo(transfrom.Rot);
	this->SetSizeInfo(transfrom.Size);	
	
	if (aObjectName != mCurrentObjectName)
	{
		mCurrentObjectName = aObjectName;
		QVector<QString> comps = Controller::GetObjectComponentList(aObjectName);	
		DeleteComponentWidget();
		for (auto val:comps)
		{
			CreateComponentWidget(val);
		}

		//QMap<QString, ComponentGroupWidget*> old_comps = mComponentWidgets;

		////遍历并生成旧的游戏物体没有的组件
		//for (auto fresh : Controller::GetObjectComponentList(aObjectName))
		//{
		//	auto iter = mComponentWidgets.find(fresh);
		//	if (iter == mComponentWidgets.end())//如果没有这个组件就生成它
		//	{
		//		CreateComponentWidget(fresh);
		//	}
		//	else//如果有，就从临时组件数组中删除
		//	{
		//		auto temp_iter = old_comps.find(iter.key());
		//		if (temp_iter!= old_comps.end())
		//		{
		//			old_comps.erase(temp_iter);
		//		}
		//		
		//	}
		//}

		////删掉所有新的游戏物体没有的组件
		//for (auto iter = old_comps.begin(); iter != old_comps.end(); iter++)
		//{
		//	DeleteComponentWidget((*iter)->title());
		//}	

	}
	
	//mTimerID = startTimer(100);
}


void InspectorWidget::CreateComponentWidget(const QString & aCompType)
{
	ComponentGroupWidget* comp = new ComponentGroupWidget(this);
	comp->setTitle(aCompType);
	mVLayout->addWidget(comp);
	mComponentWidgets.insert(comp->title(),comp);
	connect(comp, &ComponentGroupWidget::OnValueEditFinish, this, &InspectorWidget::SlotGetComponentCommand);
}

void InspectorWidget::DeleteComponentWidget(const QString & aCompType)
{
	auto iter = mComponentWidgets.find(aCompType);
	if (iter != mComponentWidgets.end())
	{
		delete iter.value();
		iter.value() = 0;
		mComponentWidgets.erase(iter);
	}
}

void InspectorWidget::DeleteComponentWidget()
{
	for (auto iter = mComponentWidgets.begin(); iter != mComponentWidgets.end();)
	{
		delete iter.value();
		iter.value() = 0;
		iter = mComponentWidgets.erase(iter);
	}

}
