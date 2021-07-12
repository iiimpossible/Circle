#pragma once
#include "GameObject.h"
#include <string>
using std::string;

class SpriteRenderer;
class GameObject2D :public GameObject
{
public:

	GameObject2D(string aName = "GameObject2D");	
	virtual ~GameObject2D();

	SpriteRenderer* SpriteRenderComp;


	virtual void Start();

	virtual void Update();
};