
#include <qdockwidget.h>
#include <vector>
#include <qvector.h>

using std::vector;

class QListWidgetItem;
class SceneListWidget;
/**
 * @brief 该widget是为了显示当前场景中的物体列表，并且可以点击物体item获得该物体的引用，对其进行操作
 * @detail 功能上要实现item的增删改查，每次增添游戏物体应该怎么更新这个列表,还有该控件和inspector的交互，
 * 因为需要点击一下条目，在inspector上显示该条目的信息
 * @parent 继承于QDockWidget
 */
class ObjectListWidget:public QDockWidget
{
	Q_OBJECT
public:

	ObjectListWidget(QWidget* aWidget = Q_NULLPTR);
	~ObjectListWidget();
public:

	/** 更新：列表中的信息，当场景加载后，需要在列表中显示场景物体*/
	void UpdateObjectList(const QVector<QString>& aNames);	
	
	/** 获得：该Dockwidget包含的widget（一个列表）*/
	inline const SceneListWidget* GetListWidget() const { return mListWidget; }

	//void RemoveSingleIte
signals:
	//与inspector的信号

	/** 信号：当列表中的项目被点击*/
	void OnListItemCliked(QString aObjetName);	
public slots:

	/** 槽函数：添加单个项目*/
	void SlotAddSingleItem(QString aObjectName);

	/** 槽函数：获得列表的被点击的项目*/
	void SlotGetListWidgetClikedItem(QListWidgetItem* aItem);

	/** 槽函数：对项目设置材质*/
	void SetObjectMaterial(const QString& aName);

	/** 槽函数：加载场景时一次性生成列表*/
	void SlotInitObjectList(const QVector<QString>& aNames);
private:

	SceneListWidget* mListWidget;
};