#pragma once
#include "Component.h"
#include <tuple>
#include "../../CCCommon/Common.h"
#include "../../CCCommon/Tool/Delegate.h"
using namespace CircleEngine;
using std::tuple;
using std::make_tuple;

/**
* @brief ��Χ�л���
* ����λ����Ϣ������Get Set����
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
	//�ж��Ƿ�������ײ���
	bool bIsIngnore;
	//�ж��ǲ��Ƕ�̬�����壬������ǣ�����̬���壬����Ҫ��������̬���������ײ��⣬ֻ��Ҫ�Ͷ�̬���������ײ���
	bool bIsDynamic;
	//�Ƿ����
	bool bDrawDebug;

	//���ԭ��
	TPosition2 Position;
	//��С
	 
	//������ɫ
	TColor3 Color;

	Delegate<void, GameObject*,Vec2> OnOverlap;
protected:
	 
};
 