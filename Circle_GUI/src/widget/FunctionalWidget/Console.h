#pragma once
#include <qdockwidget.h>
#include <qplaintextedit.h>
#include <initializer_list>
using std::initializer_list;


/**
 * @brief 这是一个控制台Widget，用于在用户界面显示一些调试信息
 * 拥有一个显示文本的Text
 */
class ConsoleWidget :public QDockWidget
{
	Q_OBJECT
public:
	ConsoleWidget(QWidget* widget = nullptr);

public:

signals:


public slots:

	void SlotAddDebugLog(const QString& aLog);

public:
	//文本操作
	
	//增
	void AddTextBack(const QString& aText);

	void AddTextBack(const initializer_list<QString>& aText);

	//删
	void ClearConsole();


protected:

	//virtual void contextMenuEvent(QContextMenuEvent *);

	//virtual void ActionsContextMenu();
private:
	QPlainTextEdit* mConsoleLog;
};