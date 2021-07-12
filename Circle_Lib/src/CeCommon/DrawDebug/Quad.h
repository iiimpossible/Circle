#pragma once
#include "../Common.h"
#include "../OpenGL/CCGLDraw.h"
#include "Shape.h"
//#include "SimpleDataStruct.h"
using CircleEngine::Vec3;
using CircleEngine::Vec2;
 /**
 * @brief �����Ǿ�����״��
 * @details  ���ΰ�Χ�У���Ҫ��С�������ǵ��Ի��ƿ��Բ���֧�����ţ���Ҫԭ�㣬��Ҫ��ɫ
 * ��λ���⣬����2D��Ⱦ�����������Ͻ�Ϊԭ�㣬����ʹ��ͼ�ε�
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
	 
	float Vertices[16];///���������DebugDraw��ʼ��

};

