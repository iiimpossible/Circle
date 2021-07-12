#include "MyDialog.h"
#include <ShaderViewer/src/widget/MyListWidget.h>
ListWidgetDialog::ListWidgetDialog(QWidget* aWidget):
	QDialog(aWidget)
{
	mListWidget = new DialogListWidget(this);
	//setWindowFlags(Qt::CustomizeWindowHint);
}