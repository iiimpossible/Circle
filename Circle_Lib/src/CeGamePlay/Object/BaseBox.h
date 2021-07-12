#pragma once
#include <string>
#include <Celib_x64/src/CeEngine/Object/GameObject.h>
#include <Celib_x64/src/CeEngine/Object/ObjectFactory.h>
using std::string;

class MeshRenderer;
class BaseBox :public GameObject
{
public:
	BaseBox(string aName = "BaseBox");
	~BaseBox();

public:

	virtual void Start();
	virtual void Update();


private:
	
};

REGISTER(BaseBox);