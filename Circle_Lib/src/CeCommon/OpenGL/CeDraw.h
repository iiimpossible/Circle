#pragma once

/*
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_TRIANGLES 0x0004
#define GL_QUADS 0x0007
#define GL_POLYGON 0x0009
*/
//ͼԪ����
enum EPel
{
	EP_POINTS = 0x0000,
	EP_LINES = 0x0001,
	EP_TRIANGLES = 0x0004,
	EP_QUADS = 0x0007,
	EP_POLYGON = 0x0009
};


//#define GL_COLOR_ATTACHMENT0 0x8CE0
//#define GL_COLOR_ATTACHMENT1 0x8CE1
//#define GL_COLOR_ATTACHMENT2 0x8CE2
//#define GL_COLOR_ATTACHMENT3 0x8CE3
//#define GL_COLOR_ATTACHMENT4 0x8CE4
//#define GL_COLOR_ATTACHMENT5 0x8CE5
//#define GL_COLOR_ATTACHMENT6 0x8CE6
//#define GL_COLOR_ATTACHMENT7 0x8CE7
//#define GL_COLOR_ATTACHMENT8 0x8CE8
//#define GL_COLOR_ATTACHMENT9 0x8CE9
//#define GL_COLOR_ATTACHMENT10 0x8CEA
//#define GL_COLOR_ATTACHMENT11 0x8CEB
//#define GL_COLOR_ATTACHMENT12 0x8CEC
//#define GL_COLOR_ATTACHMENT13 0x8CED
//#define GL_COLOR_ATTACHMENT14 0x8CEE
//#define GL_COLOR_ATTACHMENT15 0x8CEF

//��ɫ����ͨ��
enum EColorAttachment
{
	ECA_0 = 0x8CE0,
	ECA_1 = 0x8CE1,
	ECA_2 = 0x8CE2,
	ECA_3 = 0x8CE3,
	ECA_4 = 0x8CE4,
	ECA_5 = 0x8CE5,
	ECA_6 = 0x8CE6,
	ECA_7 = 0x8CE7,
	ECA_8 = 0x8CE8,
	ECA_9 = 0x8CE9,
	ECA_10 = 0x8CEA,
	ECA_11 = 0x8CEB,
	ECA_12 = 0x8CEC,
	ECA_13 = 0x8CED,
	ECA_14 = 0x8CEE,
	ECA_15 = 0x8CEF
};


/**
 * @brief ����ļ�����һЩ���Ʒ��������ƾ��Ρ�Բ�Ρ��߶ε�
 * ��������������֡���崦������������Ⱦ
 * �������ơ�ֱ�������λ���
 * �����
 */
class CeGraphDrawing
{
public:
	//��ʼ������

	/** ��ʼ��Glew������Opengl���ƿ�*/
	static int InitDrawAPI();

public:

	static void DrawGraph(unsigned aVAO, int aVerticesNum, EPel aPel = EP_TRIANGLES);

	static void DrawGraphWithIndice(unsigned aVAO, int aIndicesNum, EPel aPel = EP_TRIANGLES);

	static void DrawFramebuffer(unsigned aBufferNum,unsigned aAttachments[]);
	static void DrawFramebuffer(unsigned aBuffer);

	static void ClearBuffer();

	static void DrawFullScreen();

	static void DrawSkybox(unsigned aShaderHD,unsigned aVAO,unsigned aTextureHD,int aVerticesNum);
public:
	//һЩ���Ʒ���
	//static void DrawTriangle();

	//static void DrawLine();

	//static void DrawRectangle();

	//static void DrawCircle();


public:
	//֡���崦����
};


