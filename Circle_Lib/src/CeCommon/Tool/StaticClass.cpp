#include "StaticClass.h"
#include <typeinfo>

#include <Celib_x64/src/CeEngine/Object/Object.h>
StaticClass::StaticClass(const Object* aObject)
{

}

StaticClass::~StaticClass()
{
}

const string StaticClass::GetStaticClass(const Object & aOject)
{
	return string(typeid(aOject).name());
}
