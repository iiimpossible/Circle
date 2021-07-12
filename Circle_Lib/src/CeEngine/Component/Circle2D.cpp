#include "Circle2D.h"
#include "Box2D.h"
#include "../Object/GameObject.h"
Circle2D::Circle2D(GameObject* aOwner, TPosition2 aPosition, float aRadius, TColor3 aColor):
	BoundingBox(aOwner, aPosition, aColor),Radius(aRadius * UnitLength)
{
}

Circle2D::~Circle2D()
{
}

/*输入可能第一个是圆，也可能第二个是圆*/
tuple<bool, Vec2> Circle2D::CollisionDetection(BoundingBox* Other)
{
	 //TODO:做两个圆形包围盒之间的碰撞
	 
	Box2D* test = dynamic_cast<Box2D*>(Other);	 
	if (!test)
	{
		
		return std::make_tuple(false,Vec2());
	}	
	 
	// 获取圆的中心 
	Vec2 center = GetCenter();
	// 计算box的半边长
	Vec2 aabb_half_extents(test->GetSize().x * 0.5f, test->GetSize().y * 0.5f);
	//计算box的中心
	Vec2 aabb_center(
		test->GetBoxRelativePosition().x + aabb_half_extents.x,
		test->GetBoxRelativePosition().y + aabb_half_extents.y
	);
	// 获取两个中心的差矢量
	Vec2 difference = center - aabb_center;
	 
	Vec2 clamped = CC_Clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
	Vec2 closest = aabb_center + clamped;
	// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
	difference = closest - center;
	bool result = CC_Length(difference) < this->Radius; 
	//返回difference是为了计算球进入box的 深度 = radius - differrence.x (or difference.y)	 
	return std::make_tuple(result, difference);
}

TPosition2 Circle2D::GetBoxRelativePosition()
{
	return TPosition2(Position.x + mOwner->GetObjectPosition().x, Position.y + mOwner->GetObjectPosition().y);
}

Vec2 Circle2D::GetCenter()
{
	return Vec2(this->GetBoxRelativePosition() + this->Radius);
}

TSize2 & Circle2D::GetSize()
{
	mSize = TSize2(Radius * 2);
	return mSize;
}

void Circle2D::SetSize(float aRadius)
{
	 
	this->Radius = aRadius * UnitLength;
}
