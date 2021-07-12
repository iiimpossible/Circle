#include "Box2D.h"
#include "../Object/GameObject.h"
#include "Circle2D.h"
Box2D::Box2D(GameObject* aOwner, TPosition2 aPosition, TSize2 aSize, TColor3 aColor) :
	BoundingBox(aOwner, aPosition, aColor), Size(TSize2(aSize.x * UnitLength, aSize.y*UnitLength))
{
	
}

Box2D::~Box2D()
{

}

TPosition2 Box2D::GetBoxRelativePosition()
{
	return TPosition2(Position.x + mOwner->GetObjectPosition().x, Position.y + mOwner->GetObjectPosition().y);
}

void Box2D::SetSize(const Vec2& aSize)
{
	Size = Vec2(aSize.x * UnitLength, aSize.y * UnitLength);
}

TSize2& Box2D::GetSize()
{
	return this->Size;
}

tuple<bool, Vec2> Box2D::CollisionDetection(BoundingBox* aOther)
{
	
	Circle2D* circle = dynamic_cast<Circle2D*>(aOther);
	if (!circle)
	{
		 
		//X方向
		bool CheckX = this->GetBoxRelativePosition().x + this->Size.x >= aOther->GetBoxRelativePosition().x  &&
			aOther->GetBoxRelativePosition().x + aOther->GetSize().x >= this->GetBoxRelativePosition().x;

		//Y方向
		bool CheckY = this->GetBoxRelativePosition().y + this->Size.y >= aOther->GetBoxRelativePosition().y  &&
			aOther->GetBoxRelativePosition().y + aOther->GetSize().y >= this->GetBoxRelativePosition().y;
		bool result = CheckX && CheckY;

		return std::make_tuple(result,Vec2());
	}
	else
	{
		// 获取圆的中心 
		Vec2 center = circle->GetCenter();
		// 计算AABB的信息（中心、半边长）
		Vec2 aabb_half_extents(this->Size.x * 0.5f, this->Size.y * 0.5f);
		Vec2 aabb_center(
			this->Position.x + aabb_half_extents.x,
			this->Position.y + aabb_half_extents.y
		);
		// 获取两个中心的差矢量
		Vec2 difference = center - aabb_center;
		Vec2 clamped = CC_Clamp(difference, -aabb_half_extents, aabb_half_extents);
		// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
		Vec2 closest = aabb_center + clamped;
		// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
		difference = closest - center;
		bool result = CC_Length(difference) < circle->Radius;
		//返回difference是为了计算球进入box的 深度 = radius - differrence.x (or difference.y)
		return std::make_tuple(result, difference);
	}
 

	
}
