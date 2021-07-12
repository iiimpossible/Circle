#include "FileExplore.h"
#include <qaction.h>
#include <ShaderViewer/src/widget/MyListWidget.h>
FileExploreWidget::FileExploreWidget(QWidget* aWidget):
	QDockWidget(aWidget)
{
	mMaterialList = new MaterialListWidget(this);
	setWidget(mMaterialList);
	setWindowTitle("FileExplore");
}

FileExploreWidget::~FileExploreWidget()
{

}


void FileExploreWidget::AddListItem(const QString& aText)
{
	auto item = new QListWidgetItem(mMaterialList);
	item->setText(aText);
	mMaterialList->addItem(item);
}




void FileExploreWidget::SlotRefreshMaterialList(const QVector<QString>& aMaterials)
{
	mMaterialList->clear();
	for (auto mat_name : aMaterials)
	{
		auto item = new QListWidgetItem(mMaterialList);
		item->setText(mat_name);
		mMaterialList->addItem(item);
	}
}