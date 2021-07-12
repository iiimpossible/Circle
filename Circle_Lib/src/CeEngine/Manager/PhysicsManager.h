#pragma once
#include <iostream>
#include <vector>
#include "../../CCCommon/Common.h" 
#include "../Component/Box2D.h"
#include "../Component/Circle2D.h"
using CircleEngine::Vec2;
using CircleEngine::Vec3;
using std::vector;

//是包围盒基类
class BoundingBox;
class Box2D;
class GameObject;
class PhysicsManager
{
public:
	
	template<typename T>
	inline static T* CreateBoudingBox(GameObject* aObject)
	{
		if (!aObject)
		{
#ifdef DEBUGLOG
			DebugLog("PhysicsManager::CreateBoudingBox: GameObject Is Nullptr!");
#endif // DEBUGLOG
			return nullptr;
		}
		T* box = new T(aObject);
		BoudingBoxes.push_back(box);
		return box;
	}

	static PhysicsManager* CreatePhysicsManager();

	//static BoundingBox* CreateBoudingBox(GameObject* gameobject);
	
	void DeleteBoudingBox();

	void Update();
private:
	static vector<BoundingBox*> BoudingBoxes;
	static bool bHasCreatedPhsicsManager;

	static void CollisionDetection();

	static bool CheckAABB(BoundingBox* CurrentBox, BoundingBox* BeingTestedBox);
	PhysicsManager();
	~PhysicsManager();
};

 