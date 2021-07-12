
#include <qdockwidget.h>
#include <vector>
#include <qvector.h>

using std::vector;

class QListWidgetItem;
class SceneListWidget;
/**
 * @brief ��widget��Ϊ����ʾ��ǰ�����е������б����ҿ��Ե������item��ø���������ã�������в���
 * @detail ������Ҫʵ��item����ɾ�Ĳ飬ÿ��������Ϸ����Ӧ����ô��������б�,���иÿؼ���inspector�Ľ�����
 * ��Ϊ��Ҫ���һ����Ŀ����inspector����ʾ����Ŀ����Ϣ
 * @parent �̳���QDockWidget
 */
class ObjectListWidget:public QDockWidget
{
	Q_OBJECT
public:

	ObjectListWidget(QWidget* aWidget = Q_NULLPTR);
	~ObjectListWidget();
public:

	/** ���£��б��е���Ϣ�����������غ���Ҫ���б�����ʾ��������*/
	void UpdateObjectList(const QVector<QString>& aNames);	
	
	/** ��ã���Dockwidget������widget��һ���б�*/
	inline const SceneListWidget* GetListWidget() const { return mListWidget; }

	//void RemoveSingleIte
signals:
	//��inspector���ź�

	/** �źţ����б��е���Ŀ�����*/
	void OnListItemCliked(QString aObjetName);	
public slots:

	/** �ۺ�������ӵ�����Ŀ*/
	void SlotAddSingleItem(QString aObjectName);

	/** �ۺ���������б�ı��������Ŀ*/
	void SlotGetListWidgetClikedItem(QListWidgetItem* aItem);

	/** �ۺ���������Ŀ���ò���*/
	void SetObjectMaterial(const QString& aName);

	/** �ۺ��������س���ʱһ���������б�*/
	void SlotInitObjectList(const QVector<QString>& aNames);
private:

	SceneListWidget* mListWidget;
};