#pragma once

#include "../Common.h"
#include "../OpenGL/CCGLDraw.h"
#include <vector>
#include <string>

#include "Line.h"
//#include "SimpleDataStruct.h"
#include "Quad.h"

using CircleEngine::Vec3;
using CircleEngine::Vec2;
 

 

/*
	一个调试绘制类，其中包含了线段、矩形等形状
	这些形状已经初始化好了顶点数组和顶点缓冲，但是没有输入数据了，
	这个绘制类负责重写形状的顶点缓重

	图形的长度应该和定义的窗口的尺寸有关


*/
class DrawDebug
{
public:
	DrawDebug();
	~DrawDebug();
	static void DrawLine(Vec3 start_point = Vec3(0.0f), Vec3 end_point = Vec3(0.0f, 0.5f, 0.0f), Vec3 color = Vec3(0.0f,1.0f,0.0f));
	static void DrawQuad(Vec2 position, Vec2 size = Vec2(1.0f), Vec3 color = Vec3(0.0f, 1.0f, 0.0f));
private:
	
	 
};

 