#include "ShaderViewer.h"
#include <Windows.h>
#include <thread>
#include <qthread.h>
#include <QtWidgets/QApplication>
#include <qwindow.h>
#include "src/controller/Controller.h"
//#include <Source/CCCommon/Common.h>
int main(int argc, char *argv[])
{	
    QApplication a(argc, argv);
    ShaderViewer w;	
	Controller c;	

	//控制器和MainWidget的互相引用
	c.mMainWidget = &w;
	w.SetController(&c);
	w.PlugSocket();//MainWidge的信号和槽函数链接
	
	//获得图形窗口句柄
	HWND graphic_hd = c.GetGLWindowHandle();
	HWND center_hd;
	//获得用户界面中心Widget的句柄
	if(w.centralWidget())
		center_hd = (HWND)w.centralWidget()->winId();
	SetParent(graphic_hd, center_hd);//这个是windowsAPI

	QSize size = w.centralWidget()->size();
	//设置图形窗口的位置
	SetWindowPos(graphic_hd, HWND_TOP, 0, -46, size.width(), size.height(), SWP_FRAMECHANGED);//-45是因为菜单栏取消导致黑边，上移45个像素以去除黑边
	//style = style & (~WS_CAPTION) & ~(WS_BORDER) & ~WS_THICKFRAME;
	//SetWindowLong(glHandle, GWL_STYLE, WS_CAPTION);   //无边框无标题

	DWORD dwStyle = GetWindowLong(graphic_hd, GWL_STYLE);
	dwStyle &= ~(WS_CAPTION | WS_THICKFRAME);

	SetWindowLong(graphic_hd, GWL_STYLE, dwStyle);

	//w.showFullScreen(); //这个导致没有菜单栏
	w.ShowWidgets();
	w.showMaximized();//应该用这个
    w.show();
	
	c.ExecGameLoop();

    return a.exec();
}
