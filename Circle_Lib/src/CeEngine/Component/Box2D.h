#pragma once
#include "../../CCCommon/Common.h"
#include "../../CCCommon/Tool/Delegate.h"
#include "BoundingBox.h"

using CircleEngine::Vec2;
using CircleEngine::Vec3;
using CircleEngine::Delegate;

class GameObject;

/**
* @brief һ��2D��Χ��
*/
class Box2D :public BoundingBox
{
public:

		
	Box2D(GameObject* aOwner = nullptr,TPosition2 aPosition = TPosition2(0.0f) ,TSize2 aSize = TSize2(1.0f), TColor3 aColor = TColor3(0.0f));
	Box2D(Box2D& box2d) = delete;
	Box2D& operator=(Box2D& box2d) = delete;
	~Box2D();

	


	virtual TPosition2 GetBoxRelativePosition();
	
	void SetSize(const Vec2& aSize);
	//����ֵ��λ��һ������
	virtual TSize2& GetSize();

	virtual std::tuple<bool,Vec2> CollisionDetection(BoundingBox* aOther);
	//void SetPosition();
	//void SetColor();

	//Vec2 GetPositin();
	//Vec2 GetSize();
	//Vec2 GetColor();
	
	/**
	* @brief ��ײ���ί��
	* @param[in] GameObject* ����ð�Χ����ײ�����壨��δ���ͬʱ��ײ�����������������
	*/	
	

	 
private:
	TSize2 Size;
	
};

 