#pragma once

#include <Celib_x64/src/CeEngine/Object/GameObject.h>


class Actor :public GameObject
{
public:

	Actor(const string& aName = "Actor");
	~Actor();
public:

	virtual void Start()override;

	virtual void Update() override;
private:

};
REGISTER(Actor);
 