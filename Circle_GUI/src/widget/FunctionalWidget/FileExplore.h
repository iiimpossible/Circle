#pragma once
#include <qdockwidget.h>

class MaterialListWidget;

/**
 * @brief 文件浏览器。图形化显示已经加载到显存中的纹理、材质等资源文件
 * @detail 目前只列表显示材质已经编译加载的材质文件，未来可显示多种文件
 */
class FileExploreWidget :public QDockWidget
{
	Q_OBJECT
public:
	FileExploreWidget(QWidget* aWidget = Q_NULLPTR);
	~FileExploreWidget();

public:

	inline MaterialListWidget* GetListWidget() const { return mMaterialList; }

	void AddListItem(const QString& aText);


signals:

public slots:

	void SlotRefreshMaterialList(const QVector<QString>& aMaterials);

protected:

private:
	
	MaterialListWidget* mMaterialList;

};