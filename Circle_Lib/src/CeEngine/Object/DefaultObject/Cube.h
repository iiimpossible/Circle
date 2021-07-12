#pragma once
#include "../GameObject.h"
#include "../../../CCCommon/Tool/Structs.h"
class MeshRenderer;
class Cube:public GameObject
{
public:
	Cube(string aName = "");
	~Cube();

	virtual RenderInfo GetRenderInfo() const;
	
	MeshRenderer* MeshComp;

protected:




private:
	
	//≤‚ ‘ …æµÙ
	unsigned mTestVAO;
};

 