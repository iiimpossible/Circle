#include "Component.h"
#include <Celib_x64/src/CeEngine/Object/GameObject.h>
Component::Component(GameObject* aOwner, const string& aName):
	mOwner(aOwner),Object(aName)
{
	 
}
 
Component::~Component()
{
	mOwner = nullptr;
}



string Component::GetCommand()
{
	return mCurrentCommand;
}

GameObject * Component::GetOwner() const
{
	if (this->mOwner)
	{
		return this->mOwner;
	}
	return nullptr;
}

