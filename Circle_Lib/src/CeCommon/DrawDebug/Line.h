#pragma once

#include "../Common.h"
#include "../OpenGL/CCGLDraw.h"
#include "Shape.h"
//#include "SimpleDataStruct.h"
using CircleEngine::Vec3;
using CircleEngine::Vec2;

class line:public Shape
{
public:
	line(Vec3 color = Vec3(0.0f,1.0f,0.0f));
	~line();
	//动态改变顶点数组

	//初始化端点

 
	float* GetIndices();
	virtual void CaculateVertices(Vec3 start_point, Vec3 end_point);
public:
	Vec3 Color;
private:
	
	float EndPosints[4];

	
};



 