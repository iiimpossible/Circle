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
		 
		//X����
		bool CheckX = this->GetBoxRelativePosition().x + this->Size.x >= aOther->GetBoxRelativePosition().x  &&
			aOther->GetBoxRelativePosition().x + aOther->GetSize().x >= this->GetBoxRelativePosition().x;

		//Y����
		bool CheckY = this->GetBoxRelativePosition().y + this->Size.y >= aOther->GetBoxRelativePosition().y  &&
			aOther->GetBoxRelativePosition().y + aOther->GetSize().y >= this->GetBoxRelativePosition().y;
		bool result = CheckX && CheckY;

		return std::make_tuple(result,Vec2());
	}
	else
	{
		// ��ȡԲ������ 
		Vec2 center = circle->GetCenter();
		// ����AABB����Ϣ�����ġ���߳���
		Vec2 aabb_half_extents(this->Size.x * 0.5f, this->Size.y * 0.5f);
		Vec2 aabb_center(
			this->Position.x + aabb_half_extents.x,
			this->Position.y + aabb_half_extents.y
		);
		// ��ȡ�������ĵĲ�ʸ��
		Vec2 difference = center - aabb_center;
		Vec2 clamped = CC_Clamp(difference, -aabb_half_extents, aabb_half_extents);
		// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
		Vec2 closest = aabb_center + clamped;
		// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
		difference = closest - center;
		bool result = CC_Length(difference) < circle->Radius;
		//����difference��Ϊ�˼��������box�� ��� = radius - differrence.x (or difference.y)
		return std::make_tuple(result, difference);
	}
 

	
}
