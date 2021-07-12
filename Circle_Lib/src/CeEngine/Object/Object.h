#pragma once
#include <iostream>
#include <vector>
#include <typeinfo>
using namespace std;
#include <Celib_x64/src/CeCommon/Common.h>

/**
 * @brief ��������Ļ����࣬�ṩ������Ļ������ԡ�ʵ�ַ�����Ƶı�Ҫ��Ϣ
 */
class Object
{
public:
	
	Object(const string& aName = "Object");
	virtual ~Object()=0;

public:

	inline string GetName()const { return mName; }

	inline void SetName(const string& aName) { mName = aName; }

	/** ��̬�����жϲ������䶯̬����*/
	inline const string GetStaticClassName() const { return string(typeid(*this).name()); }	
	
protected:	
	string mName;
};

