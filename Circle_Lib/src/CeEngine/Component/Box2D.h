#pragma once
#include "../../CCCommon/Common.h"
#include "../../CCCommon/Tool/Delegate.h"
#include "BoundingBox.h"

using CircleEngine::Vec2;
using CircleEngine::Vec3;
using CircleEngine::Delegate;

class GameObject;

/**
* @brief 一个2D包围盒
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
	//返回值单位是一个像素
	virtual TSize2& GetSize();

	virtual std::tuple<bool,Vec2> CollisionDetection(BoundingBox* aOther);
	//void SetPosition();
	//void SetColor();

	//Vec2 GetPositin();
	//Vec2 GetSize();
	//Vec2 GetColor();
	
	/**
	* @brief 碰撞检测委托
	* @param[in] GameObject* 是与该包围盒碰撞的物体（如何处理同时碰撞到多个物体的情况？）
	*/	
	

	 
private:
	TSize2 Size;
	
};

 