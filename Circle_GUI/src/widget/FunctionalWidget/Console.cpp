#include "Console.h"

ConsoleWidget::ConsoleWidget(QWidget* widget):
	QDockWidget(widget)
{
	this->setWindowTitle("Log");
	this->setObjectName("Log");
	mConsoleLog = new QPlainTextEdit();
	this->setWidget(mConsoleLog);

	mConsoleLog->setPlainText("the widget is to display debug log when engine runing.");
}


void ConsoleWidget::SlotAddDebugLog(const QString& aLog)
{
	AddTextBack(aLog);
}


void ConsoleWidget::AddTextBack(const QString & aText)
{
	mConsoleLog->appendPlainText(aText);
}

void ConsoleWidget::AddTextBack(const initializer_list<QString>& aText)
{
	QString tStr;
	auto tP = aText.begin();
	while (tP!= aText.end())
	{
		tStr.push_back((*tP));
		tP++;
	}
	mConsoleLog->appendPlainText(tStr);
}

void ConsoleWidget::ClearConsole()
{
}

//void ConsoleWidget::contextMenuEvent(QContextMenuEvent *)
//{
//	mConsoleLog->setPlainText("right button menu.");
//}
