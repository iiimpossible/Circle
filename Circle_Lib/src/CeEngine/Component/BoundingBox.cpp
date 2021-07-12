#include "BoundingBox.h"
#include "../Object/GameObject.h"
 
BoundingBox::BoundingBox(GameObject* aOwner, TPosition2 aPosition, TColor3 aColor):
	Component(aOwner),Position(aPosition),Color(aColor)
{

	bIsDynamic = false;
	bIsIngnore = false;
	bDrawDebug = true;
}

BoundingBox::~BoundingBox()
{
}

tuple<bool, Vec2> BoundingBox::CollisionDetection(BoundingBox * aOther)
{
	return tuple<bool, Vec2>();
}

 

TPosition2 BoundingBox::GetBoxRelativePosition()
{
	return TPosition2();
}
 


void BoundingBox::SetBoxPosion()
{
}

void BoundingBox::SetSize()
{
}

TSize2 & BoundingBox::GetSize()
{
	return *(TSize2*)nullptr;
}

 