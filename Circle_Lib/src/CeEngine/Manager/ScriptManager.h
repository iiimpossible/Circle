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
 * @breif 脚本管理器，管理场景脚本和材质脚本
 * @detail 具有一个可添加状态的有限状态机，该状态机会执行数据识别工作
 * 具有类注册函数指针容器，遍历该容器以获得类反射信息
 * 类反射宏
 * 在实现根据反射信息产生编辑器UI的框架之前这个类没啥用，目前废弃
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
	//反射支持

	static void RegistClass(TypeRefCallback aCallback);

//	template<typename T>
	//static void GetType
 
private:

	static vector<TypeRefCallback> mTypeRefs;
	static vector<string> mTypeName;
};

 