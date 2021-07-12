#pragma once
#include "BoundingBox.h"
#include "../../CCCommon/Common.h"
#include "../../CCCommon/Tool/Delegate.h"
class Circle2D:public BoundingBox
{
public:
	Circle2D(GameObject* aOwner = nullptr, TPosition2 aPosition = TPosition2(0.0f), float aRadius = 1.0f, TColor3 aColor = TColor3(0.0f));
	~Circle2D();

	virtual tuple<bool,Vec2> CollisionDetection(BoundingBox* aOther);

	//ע�����λ���ǰ�ΧCircle����С���ε����Ͻ�
	virtual TPosition2 GetBoxRelativePosition();

	Vec2 GetCenter();
	virtual TSize2& GetSize();
	//ע���ǰ뾶����λ��UnitLength
	virtual void SetSize(float aRadius);
	float Radius;

	 
private:

	//���ģ���ȥ��
	TSize2 mSize;

	
};

 