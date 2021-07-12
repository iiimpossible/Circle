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

/*������ܵ�һ����Բ��Ҳ���ܵڶ�����Բ*/
tuple<bool, Vec2> Circle2D::CollisionDetection(BoundingBox* Other)
{
	 //TODO:������Բ�ΰ�Χ��֮�����ײ
	 
	Box2D* test = dynamic_cast<Box2D*>(Other);	 
	if (!test)
	{
		
		return std::make_tuple(false,Vec2());
	}	
	 
	// ��ȡԲ������ 
	Vec2 center = GetCenter();
	// ����box�İ�߳�
	Vec2 aabb_half_extents(test->GetSize().x * 0.5f, test->GetSize().y * 0.5f);
	//����box������
	Vec2 aabb_center(
		test->GetBoxRelativePosition().x + aabb_half_extents.x,
		test->GetBoxRelativePosition().y + aabb_half_extents.y
	);
	// ��ȡ�������ĵĲ�ʸ��
	Vec2 difference = center - aabb_center;
	 
	Vec2 clamped = CC_Clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
	Vec2 closest = aabb_center + clamped;
	// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
	difference = closest - center;
	bool result = CC_Length(difference) < this->Radius; 
	//����difference��Ϊ�˼��������box�� ��� = radius - differrence.x (or difference.y)	 
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
