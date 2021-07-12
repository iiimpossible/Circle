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

	//��������MainWidget�Ļ�������
	c.mMainWidget = &w;
	w.SetController(&c);
	w.PlugSocket();//MainWidge���źźͲۺ�������
	
	//���ͼ�δ��ھ��
	HWND graphic_hd = c.GetGLWindowHandle();
	HWND center_hd;
	//����û���������Widget�ľ��
	if(w.centralWidget())
		center_hd = (HWND)w.centralWidget()->winId();
	SetParent(graphic_hd, center_hd);//�����windowsAPI

	QSize size = w.centralWidget()->size();
	//����ͼ�δ��ڵ�λ��
	SetWindowPos(graphic_hd, HWND_TOP, 0, -46, size.width(), size.height(), SWP_FRAMECHANGED);//-45����Ϊ�˵���ȡ�����ºڱߣ�����45��������ȥ���ڱ�
	//style = style & (~WS_CAPTION) & ~(WS_BORDER) & ~WS_THICKFRAME;
	//SetWindowLong(glHandle, GWL_STYLE, WS_CAPTION);   //�ޱ߿��ޱ���

	DWORD dwStyle = GetWindowLong(graphic_hd, GWL_STYLE);
	dwStyle &= ~(WS_CAPTION | WS_THICKFRAME);

	SetWindowLong(graphic_hd, GWL_STYLE, dwStyle);

	//w.showFullScreen(); //�������û�в˵���
	w.ShowWidgets();
	w.showMaximized();//Ӧ�������
    w.show();
	
	c.ExecGameLoop();

    return a.exec();
}
