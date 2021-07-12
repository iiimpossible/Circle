#pragma once 
#include <windows.h>
#include <iostream>
#include "../Common.h"

using namespace CircleEngine;
struct GLFWwindow;
typedef void(*CeWindowResiseCallback)(GLFWwindow*, int, int);

//#define GL_DEPTH_TEST                     0x0B71
//#define GL_DEPTH_WRITEMASK                0x0B72
//#define GL_DEPTH_CLEAR_VALUE              0x0B73
//#define GL_DEPTH_FUNC                     0x0B74
//#define GL_ACCUM_CLEAR_VALUE              0x0B80
//#define GL_STENCIL_TEST                   0x0B90
//#define GL_BLEND                          0x0BE2
enum EEnalbe
{
	EEB_DepthTest = 0x0B71,
	EEB_StencilTest = 0x0B90,
	EEB_Blend = 0x0BE2
};

/**
 * @breif CircleEngineͼ�δ�����
 * @detail 
 */
class CeGraphWindow 
{
public:
	//����
	CeGraphWindow(int aW = 800, int aH = 600);
	~CeGraphWindow();
public:

	/** @breif ����һ��ͼ�δ��� */
	static CeGraphWindow* CreateGLWindow();

	static int InitWindowAPI();

	static void Terminate();
public:

	/** @breif ���ͼ�δ���windows��� */
	HWND GetWinHandle();

	/** @breif ���GLFW����ָ��*/
	static GLFWwindow* GetGLFWWindow();	

	/** @breif ���CircleEngine��װ��ͼ�δ���ָ��*/
	static CeGraphWindow* GetGLWindow();

public:
	//�������Է���

	/** @breif �޸Ĵ��ڴ�С*/
	static void ResizeWindow(int aWidth, int aHeight);
	/** @breif ���ô���λ��*/
	static void SetWindowPosition(int aX, int aY);
	/** @breif ��ô�������*/
	inline static Vec2 GetWindowSize() { return Vec2(mWidth, mHeight); }

	static void SetViewport(const Vec2& aSize);
	static void SetViewport(int aWidth, int aHeight);

public:
	//��Ⱦ����

	/** @breif ������Ⱦ���ԣ���ȡ�ģ��ȣ�*/
	static void EnableTest(EEnalbe aTest);

	/** @breif �ر���Ⱦ����*/
	static void DisableTest(EEnalbe aTest);

	/** @breif û�йر���Ȳ��Ե��ǽ������ֵд��*/
	static void DisableDepthWrite();
	static void EnableDepthWrite();

public:
	//�¼�	

private:

	static void BindOpenGLCallback();

	static void FrameBufferSizeCallback(GLFWwindow*, int, int);

	HWND InitGLWindow();
private:
	//����
	HWND mHWND;
	static GLFWwindow * mGLFWWindow;	
	static CeGraphWindow* mCeWindow;
	static int mWidth;
	static int mHeight;
	static bool isInit;
};

