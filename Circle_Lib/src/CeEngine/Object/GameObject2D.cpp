#include "GameObject2D.h"
#include "../Component/SpriteRenderer.h"


GameObject2D::GameObject2D(string aName) :
	GameObject(aName)
{
	//将组件类型信息注册到map中
	SpriteRenderComp = AddComponent<SpriteRenderer>(new SpriteRenderer(this));
}

GameObject2D::~GameObject2D()
{
}


void GameObject2D::Start()
{
}

void GameObject2D::Update()
{
}
