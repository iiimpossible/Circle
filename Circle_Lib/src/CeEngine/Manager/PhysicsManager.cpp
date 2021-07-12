#include "PhysicsManager.h"
#include <tuple>
#include "../Object/GameObject.h"
#include "../../CCCommon/DrawDebug/DrawDebug.h"
#include "../Component/BoundingBox.h"

vector<BoundingBox*> PhysicsManager::BoudingBoxes;
bool PhysicsManager::bHasCreatedPhsicsManager;

PhysicsManager * PhysicsManager::CreatePhysicsManager()
{
	if (!bHasCreatedPhsicsManager)
	{
		bHasCreatedPhsicsManager = true;
		return new PhysicsManager();
	}
#ifdef DEBUGLOG
	DebugLog("PhysicsManager::CreatePhysicsManager: PhysicsManager Has Created!");
#endif // 	
	return nullptr;	
}

//BoundingBox * PhysicsManager::CreateBoudingBox(GameObject* gameobject)
//{
//	
//	if (!gameobject)
//	{
//#ifdef DEBUGLOG
//		DebugLog("PhysicsManager::CreateBoudingBox: GameObject Is Nullptr!");
//#endif // DEBUGLOG
//		return nullptr;
//	}
//	BoundingBox* box = new BoundingBox(gameobject);
//	BoudingBoxes.push_back(box);
//	return box;
//	 
//}
/**
* @brief Release memory of a box
* 根据什么来查找包围盒呢？什么时候释放内存呢？由GameObject释放还是又管理器释放？
* 对于没有管理器的组件又如何释放呢？
*/
void PhysicsManager::DeleteBoudingBox()
{
}

void PhysicsManager::Update()
{
	CollisionDetection();
}
/**
* @brief 遍历容器以进行碰撞检测
* TODO：使用四叉树优化
*/
void PhysicsManager::CollisionDetection()
{
	
	//这样会导致自己和自己进行碰撞检测
	for (int i = 0; i <BoudingBoxes.size(); i++)
	{	
		////是否开启绘制包围盒
		//if (BoudingBoxes[i]->bDrawDebug)
		//{
		//	DrawDebug::DrawQuad(BoudingBoxes[i]->GetBoxRelativePosition(), Vec2(BoudingBoxes[i]->GetSize().x, BoudingBoxes[i]->GetSize().y));
		//}
		//不检测被忽视的物体
		if (BoudingBoxes[i]->bIsIngnore)
		{
			continue;
		}
		//如果不是动态物体，则不检测
		if (!BoudingBoxes[i]->bIsDynamic)
		{
			continue;
		}
		
		//动态物体和其他物体进行碰撞检测
		for (int j = 0; j < BoudingBoxes.size();j++)
		{
			//不检测被忽视的物体
			if (BoudingBoxes[j]->bIsIngnore)
			{
				continue;
			}
			//不和自己进行碰撞检测
			if (j == i)
			{
				continue;
			}
			std::tuple<bool, Vec2> t = BoudingBoxes[i]->CollisionDetection(BoudingBoxes[j]);
			if (std::get<0>(t))
			{
				GameObject* object = BoudingBoxes[j]->GetOwner();
				if (object)
				{					 
					//执行回调函数					
					BoudingBoxes[i]->OnOverlap(object, std::get<1>(t));
				}  
				
			}
		}
	}
}


/**
* @brief AABB碰撞检测
* @note 目前碰撞后的简单物理效果比如：反弹（改变方向），由Game Object实现。
*/
bool PhysicsManager::CheckAABB(BoundingBox* CurrentBox, BoundingBox* BeingTestedBox)
{
	if (!CurrentBox || !BeingTestedBox)
	{
#ifdef DEBUGLOG
		DebugLog("@PhysicsManager::CheckAABB: ERROR: Bouding Box Nullptr!  ")
#endif // DEBUGLOG

		return false;
	}
	
	//X方向
	bool CheckX = CurrentBox->GetBoxRelativePosition().x + CurrentBox->GetSize().x >= BeingTestedBox->GetBoxRelativePosition().x  &&
		BeingTestedBox->GetBoxRelativePosition().x + BeingTestedBox->GetSize().x >= CurrentBox->GetBoxRelativePosition().x;

	//Y方向
	bool CheckY = CurrentBox->GetBoxRelativePosition().y + CurrentBox->GetSize().y >= BeingTestedBox->GetBoxRelativePosition().y  &&
		BeingTestedBox->GetBoxRelativePosition().y + BeingTestedBox->GetSize().y >= CurrentBox->GetBoxRelativePosition().y;
	bool result = CheckX && CheckY;
	 
	return result;
}

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}
