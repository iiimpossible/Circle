#pragma once
#include <iostream>
#include <vector>
#include <typeinfo>
using namespace std;
#include <Celib_x64/src/CeCommon/Common.h>

/**
 * @brief 整个引擎的基础类，提供了物体的基本属性、实现反射机制的必要信息
 */
class Object
{
public:
	
	Object(const string& aName = "Object");
	virtual ~Object()=0;

public:

	inline string GetName()const { return mName; }

	inline void SetName(const string& aName) { mName = aName; }

	/** 动态类型判断并返回其动态类型*/
	inline const string GetStaticClassName() const { return string(typeid(*this).name()); }	
	
protected:	
	string mName;
};

