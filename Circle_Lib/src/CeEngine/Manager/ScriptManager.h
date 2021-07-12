#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;
#define CeClass()						 \
static inline string GetTypeName()		 \
{										 \
	return GetStaticClassName();		 \
}										 \
ScriptManager::RegistClass(GetTypeName); 

#define CeStruct()
#define CeProperty()
#define CeFunction()

typedef string(*TypeRefCallback)();
/**
 * @breif �ű����������������ű��Ͳ��ʽű�
 * @detail ����һ�������״̬������״̬������״̬����ִ������ʶ����
 * ������ע�ắ��ָ�������������������Ի���෴����Ϣ
 * �෴���
 * ��ʵ�ָ��ݷ�����Ϣ�����༭��UI�Ŀ��֮ǰ�����ûɶ�ã�Ŀǰ����
 */
class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();

public:

	void Start();

	void Update();
public:
	//����֧��

	static void RegistClass(TypeRefCallback aCallback);

//	template<typename T>
	//static void GetType
 
private:

	static vector<TypeRefCallback> mTypeRefs;
	static vector<string> mTypeName;
};

 