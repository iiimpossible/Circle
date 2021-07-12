#pragma once
#include <string>
#include <Celib_x64/src/CeEngine/Object/GameObject.h>
 
using std::string;


class Cube :public GameObject
{
public:

	Cube(const string& aName = "Cube");
	~Cube();
public:

	virtual void Start() override;
	virtual void Update()override;

	
private:
	
};
REGISTER(Cube);