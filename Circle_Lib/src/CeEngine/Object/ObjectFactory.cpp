#include "ObjectFactory.h"
#include "GameObject.h"
#include "../Manager/DebugLogManager.h"
using std::pair;


unordered_map<string, ObjectCreateCallBack> ObjectFactory::mObjectTypeNameMap;

ClassRegister::ClassRegister(const string& aObjectType, ObjectCreateCallBack aFactoryFunc)
{
	ObjectFactory::RegistObjectType(aObjectType, aFactoryFunc);
}



int ObjectFactory::RegistObjectType(const string & aObjectType, ObjectCreateCallBack aFactoryFunc)
{
	//����
	if (mObjectTypeNameMap.find(aObjectType) == mObjectTypeNameMap.end())
	{
		mObjectTypeNameMap.insert(pair<string, ObjectCreateCallBack>(aObjectType, aFactoryFunc));
		return 1;
	}	
	return 0;
}

GameObject* ObjectFactory::GetObjectInstance(const string & aObjectType)
{
	auto iter = mObjectTypeNameMap.find(aObjectType);
	if (iter != mObjectTypeNameMap.end())
	{
		auto obj = (*iter).second();//������Ļص��������ɶ�Ӧʵ��
		if (!obj)
		{
			DebugLogManager::DebugOutput("ObjectFactorry", EDebugTag::EDT_ERROR, {"Create object failed, perhaps callback func exception."});
			return nullptr;
		}
		DebugLogManager::DebugOutput("ObjectFactorry", EDebugTag::EDT_Log, { "Object created!" });
		return static_cast<GameObject*>(obj);//���ص��Ǻ���ָ��ִ�еĽ��
	}		
	else
	{
		DebugLogManager::DebugOutput("ObjectFactorry",EDebugTag::EDT_ERROR, { "Create object failed. Object type is: ",aObjectType });
		return nullptr;
	}
		
}

vector<string> ObjectFactory::GetGameObjectTypeNames()
{
	vector<string> temp_array;
	std::unordered_map<string, ObjectCreateCallBack>::iterator iter = mObjectTypeNameMap.begin();
	while (iter != mObjectTypeNameMap.end())
	{
		temp_array.push_back((*iter).first);
		iter++;
	}
	return temp_array;
}
