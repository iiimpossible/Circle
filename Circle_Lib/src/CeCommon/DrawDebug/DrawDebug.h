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
	һ�����Ի����࣬���а������߶Ρ����ε���״
	��Щ��״�Ѿ���ʼ�����˶�������Ͷ��㻺�壬����û�����������ˣ�
	��������ฺ����д��״�Ķ��㻺��

	ͼ�εĳ���Ӧ�úͶ���Ĵ��ڵĳߴ��й�


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

 