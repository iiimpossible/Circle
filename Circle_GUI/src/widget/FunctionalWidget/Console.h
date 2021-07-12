#pragma once
#include <qdockwidget.h>
#include <qplaintextedit.h>
#include <initializer_list>
using std::initializer_list;


/**
 * @brief ����һ������̨Widget���������û�������ʾһЩ������Ϣ
 * ӵ��һ����ʾ�ı���Text
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
	//�ı�����
	
	//��
	void AddTextBack(const QString& aText);

	void AddTextBack(const initializer_list<QString>& aText);

	//ɾ
	void ClearConsole();


protected:

	//virtual void contextMenuEvent(QContextMenuEvent *);

	//virtual void ActionsContextMenu();
private:
	QPlainTextEdit* mConsoleLog;
};