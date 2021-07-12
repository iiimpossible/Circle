#pragma once
#include <CeEngine/Object/GameObject.h>


class Earth:public GameObject
{
public:
	Earth(const string& aName = "Earth");
	~Earth();


public:

	virtual void Start() override;
	virtual void Update()override;

public:

	//绕恒星质心公转
	void RevolveAroundSun(const Vec3& aCenter, float aRadius = 100.0f, float aAngularV = 1.0f);




private:

};
REGISTER(Earth);

 