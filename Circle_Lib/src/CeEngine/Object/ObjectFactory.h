#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using std::unordered_map;
using std::string;
using std::vector;

class Object;
class GameObject;
class ClassRegister;
typedef void*(*ObjectCreateCallBack)(void);

/**
 * @brief ��������Ϸ����Ĺ��������ࡣͨ�������ַ������������ƥ�����Ϸ����
 * ����Ӧ����һ�������洢��ע���Ѿ�д�õ���
 */
class ObjectFactory
{

public:

	/** @breif ע��һ����Ϸ�������͵�����*/
	static int RegistObjectType(const string& aObjectType = "", ObjectCreateCallBack aFactoryFunc = nullptr);

	/** @breif ����һ����Ӧ�����ʵ��*/
	static GameObject* GetObjectInstance(const string& aObjectType);

	/** @breif ���������Ѿ�ע�����Ϸ��������*/
	static vector<string> GetGameObjectTypeNames() ;
private :

	static unordered_map<string, ObjectCreateCallBack> mObjectTypeNameMap;
};


/**
 * @brief �������ڸ���Ĺ��췽����ִ��ע������Ϣ�����
 * ���������ඨ��ĺ������һ��ȫ�ֱ�����ע����Ϣ
 * ԭ��ȫ���Զ���������main����֮ǰִ�У��Ϳ��԰�����Ϣע�ᵽ������
 */
class ClassRegister
{
public:
	// @brief ����Ϊ������������������
	ClassRegister(const string& aObjectType, ObjectCreateCallBack aFactoryFunc);
	//ClassRigister(string aObjectType, ObjectCreateCallBack aFactoryFunc);
};

/**
 * @brief ����ǰ��ĺ�����һ����������ɺ�������ߵ���һ��ע����ʵ������������Ϣ�ͺ���ָ��ע�ᵽ������
 * ##�ǽ������ֱ�Ӻ�##ǰ�ߵķ�������
 * #�ǽ������תΪ�ַ���
 */
#define REGISTER(TypeName)											\
	TypeName* ObjectCreate##TypeName(){								\
		return new TypeName();}										\
	 ClassRegister gClassRigister##TypeName(						\
		#TypeName, (ObjectCreateCallBack)ObjectCreate##TypeName)