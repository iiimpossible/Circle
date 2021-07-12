#pragma once
#include "GameObject.h"

class SpriteRenderer;
/**
* @brief 该类是为了学习图形算法，渲染实时动画（无顶点）
*/
class Canvas :public GameObject
{
public:
	Canvas(string aName = "Canvas");
	~Canvas();
public:
	float mWidth;
	float mHeight;

	virtual RenderInfo GetRenderInfo();
	SpriteRenderer* RendererComp;

private:

};

 