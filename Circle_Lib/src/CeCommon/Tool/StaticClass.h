#pragma once
#include <string>
#include "../Common.h"

using std::string;
class Object;
/**
 * @brief ��¼�̳в���µľ�̬�����Խ���Object�Ķ�̬�����ж�
 * �������ʱû��
 */
class StaticClass
{
public:
	StaticClass(const Object* aObject);
	~StaticClass();

	static const string GetStaticClass(const Object& aOject);
};