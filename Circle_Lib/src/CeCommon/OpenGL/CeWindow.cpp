#include "CeWindow.h"
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
//#include <Celib_x64/src/CeCommon/OpenGL/CeDraw.h>
int CeGraphWindow::mWidth = 800;
int  CeGraphWindow::mHeight = 600;
CeGraphWindow* CeGraphWindow::mCeWindow;
GLFWwindow * CeGraphWindow::mGLFWWindow;
bool CeGraphWindow::isInit = false;
CeGraphWindow::CeGraphWindow(int aW, int aH)
{
	mWidth = aW;
	mHeight = aH;
	mHWND = InitGLWindow();
}


CeGraphWindow::~CeGraphWindow(){}

int CeGraphWindow::InitWindowAPI()
{
	int tag = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//版本设置，大版本号和和小版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//opengl 核心配置
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);//窗口是否可以改变大小
	//glfwWindowHint(GLFW_SAMPLES, 4);//开启多重采样，没用？
	


	if (!tag)
		return 0;
	return 1;
}

void CeGraphWindow::Terminate()
{
	glfwTerminate();
}

HWND CeGraphWindow::InitGLWindow()
{
	mGLFWWindow = glfwCreateWindow(mWidth, mHeight, "GraphicWindow", nullptr, nullptr);
	if (mGLFWWindow == nullptr)//如果窗口创建失败，结束窗口创建	
	{
		glfwTerminate();
		return 0;
	}		

	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//{
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return 0;
	//}


	HWND win_hd = glfwGetWin32Window(mGLFWWindow);	//获得图形窗口的window句柄	
	glfwMakeContextCurrent(mGLFWWindow);//使得某个图形窗口为当前上下文处理的窗口
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600);
	//glfwSetWindowSize(mGLFWWindow,800,600);
	//glfwSetFramebufferSizeCallback(mGLFWWindow, FrameBufferSizeCallback);
	//glfwSetInputMode(mGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //捕捉鼠标在窗口内，不让他显示
	BindOpenGLCallback();
	return win_hd;
}

HWND CeGraphWindow::GetWinHandle()
{
	return mHWND;
}
GLFWwindow*  CeGraphWindow::GetGLFWWindow()
{
	return mGLFWWindow;
}

CeGraphWindow* CeGraphWindow:: CreateGLWindow()
{
	mCeWindow = new CeGraphWindow();
	return mCeWindow;
}



CeGraphWindow* CeGraphWindow::GetGLWindow()
{
	return mCeWindow;
}

 

void CeGraphWindow::BindOpenGLCallback()
{
	//窗口大小
	glfwSetFramebufferSizeCallback(mGLFWWindow, FrameBufferSizeCallback);
}

void CeGraphWindow::ResizeWindow(int aWidth, int aHeight)
{
	glfwSetWindowSize(mGLFWWindow, aWidth, aHeight);
	glViewport(0, 0, aWidth, aHeight);
	mWidth = aWidth;
	mHeight = aHeight;
}

void CeGraphWindow::SetWindowPosition(int aX, int aY)
{
	//SetWindowPos(mHWND,);
}

void CeGraphWindow::SetViewport(const Vec2& aSize)
{
	glViewport(0, 0, aSize.x, aSize.y);
}

void CeGraphWindow::SetViewport(int aWidth, int aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
}

void CeGraphWindow::EnableTest(EEnalbe aTest)
{
//#define GL_SCISSOR_TEST                   0x0C11 
//#define GL_ALPHA_TEST                     0x0BC0
//#define GL_DEPTH_TEST                     0x0B71
//#define GL_STENCIL_TEST                   0x0B90
	//开启Opengl的一些功能
	glEnable(aTest);
	
	//glEnable(0x809D);//开启多重采样？
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CeGraphWindow::DisableTest(EEnalbe aTest)
{
	glDisable(aTest);
}

void CeGraphWindow::DisableDepthWrite()
{
	glDepthMask(GL_FALSE);
}

void CeGraphWindow::EnableDepthWrite()
{
	glDepthMask(GL_TRUE);
}

void CeGraphWindow::FrameBufferSizeCallback(GLFWwindow *, int aWidth, int aHeight)
{
	//mWidth = aWidth;
	//mHeight = aHeight;
	//glViewport(0, 0, mWidth, mHeight);
}
