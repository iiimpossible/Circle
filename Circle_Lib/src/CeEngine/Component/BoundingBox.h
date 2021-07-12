#pragma once
#include "Component.h"
#include <tuple>
#include "../../CCCommon/Common.h"
#include "../../CCCommon/Tool/Delegate.h"
using namespace CircleEngine;
using std::tuple;
using std::make_tuple;

/**
* @brief 包围盒基类
* 具有位置信息，及其Get Set方法
* 
*/
class BoundingBox:public Component
{
public:
	 

	BoundingBox(GameObject* aOwner = nullptr, TPosition2 aPosition = TPosition2(0.0f), TColor3 aColor = TColor3(0.0f));
	BoundingBox(BoundingBox& aBoundingBox) = delete;
	BoundingBox& operator=(BoundingBox& aBoundingBox) = delete;
	virtual ~BoundingBox();

	virtual tuple<bool, Vec2> CollisionDetection(BoundingBox* aOther);

	virtual TPosition2 GetBoxRelativePosition();
	void SetBoxPosion();
	virtual void SetSize();

	virtual TSize2& GetSize();
	//判断是否作用碰撞检测
	bool bIsIngnore;
	//判断是不是动态的物体，如果不是，即静态物体，不需要和其他静态物体进行碰撞检测，只需要和动态物体进行碰撞检测
	bool bIsDynamic;
	//是否绘制
	bool bDrawDebug;

	//相对原点
	TPosition2 Position;
	//大小
	 
	//绘制颜色
	TColor3 Color;

	Delegate<void, GameObject*,Vec2> OnOverlap;
protected:
	 
};
 