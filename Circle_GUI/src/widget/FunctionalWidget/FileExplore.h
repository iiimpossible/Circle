#pragma once
#include <qdockwidget.h>

class MaterialListWidget;

/**
 * @brief �ļ��������ͼ�λ���ʾ�Ѿ����ص��Դ��е��������ʵ���Դ�ļ�
 * @detail Ŀǰֻ�б���ʾ�����Ѿ�������صĲ����ļ���δ������ʾ�����ļ�
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