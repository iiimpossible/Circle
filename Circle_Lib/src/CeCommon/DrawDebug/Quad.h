#pragma once
#include "../Common.h"
#include "../OpenGL/CCGLDraw.h"
#include "Shape.h"
//#include "SimpleDataStruct.h"
using CircleEngine::Vec3;
using CircleEngine::Vec2;
 /**
 * @brief 该类是矩形形状类
 * @details  矩形包围盒，需要大小，由于是调试绘制可以不用支持缩放，需要原点，需要颜色
 * 定位问题，由于2D渲染的坐标是左上角为原点，所以使用图形的
 */

class Quad:public Shape
{
public:
	Quad(Vec2 position = Vec2(0.0f), Vec2 size = Vec3(1.0f), Vec3 color = Vec3(0.0f,1.0f,0.0f));
	~Quad();
	
	float* GetVertices();
	virtual void CaculateVertices(Vec2 position, Vec2 size);
public:
	Vec3 Color;
private:
	
	Vec2 Size;
	Vec2 Position;
	 
	float Vertices[16];///这个数组由DebugDraw初始化

};

