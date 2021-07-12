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
 * @brief 该类是游戏物体的工厂方法类。通过输入字符串类型来获得匹配的游戏物体
 * 该类应该有一个容器存储或注册已经写好的类
 */
class ObjectFactory
{

public:

	/** @breif 注册一个游戏物体类型到工厂*/
	static int RegistObjectType(const string& aObjectType = "", ObjectCreateCallBack aFactoryFunc = nullptr);

	/** @breif 返回一个对应对象的实例*/
	static GameObject* GetObjectInstance(const string& aObjectType);

	/** @breif 返回所有已经注册的游戏物体类型*/
	static vector<string> GetGameObjectTypeNames() ;
private :

	static unordered_map<string, ObjectCreateCallBack> mObjectTypeNameMap;
};


/**
 * @brief 这个类会在该类的构造方法中执行注册类信息的语句
 * 该类是在类定义的后边声明一个全局变量以注册信息
 * 原理：全局自动变量会在main函数之前执行，就可以把类信息注册到容器中
 */
class ClassRegister
{
public:
	// @brief 参数为：类型名，工厂方法
	ClassRegister(const string& aObjectType, ObjectCreateCallBack aFactoryFunc);
	//ClassRigister(string aObjectType, ObjectCreateCallBack aFactoryFunc);
};

/**
 * @brief 以下前面的函数是一个类对象生成函数，后边的是一个注册类实例，将类型信息和函数指针注册到工厂中
 * ##是将宏参数直接和##前边的符号连接
 * #是将宏参数转为字符串
 */
#define REGISTER(TypeName)											\
	TypeName* ObjectCreate##TypeName(){								\
		return new TypeName();}										\
	 ClassRegister gClassRigister##TypeName(						\
		#TypeName, (ObjectCreateCallBack)ObjectCreate##TypeName)