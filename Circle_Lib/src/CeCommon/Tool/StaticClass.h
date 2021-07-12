#pragma once
#include <string>
#include "../Common.h"

using std::string;
class Object;
/**
 * @brief 记录继承层次下的静态类型以进行Object的动态类型判断
 * 这个类暂时没用
 */
class StaticClass
{
public:
	StaticClass(const Object* aObject);
	~StaticClass();

	static const string GetStaticClass(const Object& aOject);
};