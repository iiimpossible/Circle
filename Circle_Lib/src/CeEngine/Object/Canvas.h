#pragma once
#include "GameObject.h"

class SpriteRenderer;
/**
* @brief ������Ϊ��ѧϰͼ���㷨����Ⱦʵʱ�������޶��㣩
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

 