#pragma once
#include <qdialog.h>
class DialogListWidget;
class ListWidgetDialog :public QDialog
{
	//Q_OBJECT
public:
	ListWidgetDialog(QWidget* aWidget = Q_NULLPTR);

public:

	inline DialogListWidget* GetListWidget()const { return mListWidget; }
signals:

public slots:

private:

	DialogListWidget* mListWidget;
};