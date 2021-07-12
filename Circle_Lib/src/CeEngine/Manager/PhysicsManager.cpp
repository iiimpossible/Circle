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
* ����ʲô�����Ұ�Χ���أ�ʲôʱ���ͷ��ڴ��أ���GameObject�ͷŻ����ֹ������ͷţ�
* ����û�й����������������ͷ��أ�
*/
void PhysicsManager::DeleteBoudingBox()
{
}

void PhysicsManager::Update()
{
	CollisionDetection();
}
/**
* @brief ���������Խ�����ײ���
* TODO��ʹ���Ĳ����Ż�
*/
void PhysicsManager::CollisionDetection()
{
	
	//�����ᵼ���Լ����Լ�������ײ���
	for (int i = 0; i <BoudingBoxes.size(); i++)
	{	
		////�Ƿ������ư�Χ��
		//if (BoudingBoxes[i]->bDrawDebug)
		//{
		//	DrawDebug::DrawQuad(BoudingBoxes[i]->GetBoxRelativePosition(), Vec2(BoudingBoxes[i]->GetSize().x, BoudingBoxes[i]->GetSize().y));
		//}
		//����ⱻ���ӵ�����
		if (BoudingBoxes[i]->bIsIngnore)
		{
			continue;
		}
		//������Ƕ�̬���壬�򲻼��
		if (!BoudingBoxes[i]->bIsDynamic)
		{
			continue;
		}
		
		//��̬������������������ײ���
		for (int j = 0; j < BoudingBoxes.size();j++)
		{
			//����ⱻ���ӵ�����
			if (BoudingBoxes[j]->bIsIngnore)
			{
				continue;
			}
			//�����Լ�������ײ���
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
					//ִ�лص�����					
					BoudingBoxes[i]->OnOverlap(object, std::get<1>(t));
				}  
				
			}
		}
	}
}


/**
* @brief AABB��ײ���
* @note Ŀǰ��ײ��ļ�����Ч�����磺�������ı䷽�򣩣���Game Objectʵ�֡�
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
	
	//X����
	bool CheckX = CurrentBox->GetBoxRelativePosition().x + CurrentBox->GetSize().x >= BeingTestedBox->GetBoxRelativePosition().x  &&
		BeingTestedBox->GetBoxRelativePosition().x + BeingTestedBox->GetSize().x >= CurrentBox->GetBoxRelativePosition().x;

	//Y����
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
