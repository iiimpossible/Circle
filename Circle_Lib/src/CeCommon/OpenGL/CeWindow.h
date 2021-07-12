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
 * @breif CircleEngine图形窗口类
 * @detail 
 */
class CeGraphWindow 
{
public:
	//方法
	CeGraphWindow(int aW = 800, int aH = 600);
	~CeGraphWindow();
public:

	/** @breif 创建一个图形窗口 */
	static CeGraphWindow* CreateGLWindow();

	static int InitWindowAPI();

	static void Terminate();
public:

	/** @breif 获得图形窗口windows句柄 */
	HWND GetWinHandle();

	/** @breif 获得GLFW窗口指针*/
	static GLFWwindow* GetGLFWWindow();	

	/** @breif 获得CircleEngine封装的图形窗口指针*/
	static CeGraphWindow* GetGLWindow();

public:
	//窗口属性方法

	/** @breif 修改窗口大小*/
	static void ResizeWindow(int aWidth, int aHeight);
	/** @breif 设置窗口位置*/
	static void SetWindowPosition(int aX, int aY);
	/** @breif 获得窗口坐标*/
	inline static Vec2 GetWindowSize() { return Vec2(mWidth, mHeight); }

	static void SetViewport(const Vec2& aSize);
	static void SetViewport(int aWidth, int aHeight);

public:
	//渲染设置

	/** @breif 开启渲染测试（深度、模板等）*/
	static void EnableTest(EEnalbe aTest);

	/** @breif 关闭渲染测试*/
	static void DisableTest(EEnalbe aTest);

	/** @breif 没有关闭深度测试但是禁用深度值写入*/
	static void DisableDepthWrite();
	static void EnableDepthWrite();

public:
	//事件	

private:

	static void BindOpenGLCallback();

	static void FrameBufferSizeCallback(GLFWwindow*, int, int);

	HWND InitGLWindow();
private:
	//属性
	HWND mHWND;
	static GLFWwindow * mGLFWWindow;	
	static CeGraphWindow* mCeWindow;
	static int mWidth;
	static int mHeight;
	static bool isInit;
};

