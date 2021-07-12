#pragma once
#include <string>
#include <Celib_x64/src/CeEngine/Object/GameObject.h>
//#include <CeEngine/Object/ObjectFactory.h>
using std::string;

class LightComponent;
class DirLight :public GameObject
{
public:
	DirLight(const string& aName = "DirLight");
	~DirLight();
	
public:


public:

	virtual void Start() override;
	virtual void Update()override;

private:

	LightComponent* mLightComp;
};
REGISTER(DirLight);