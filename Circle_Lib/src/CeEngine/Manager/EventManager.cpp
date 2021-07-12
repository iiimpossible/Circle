#include <Windows.h>
#include "EventManager.h"

#include "../../CCCommon/Common.h"

Delegate<void, int>EventManager::OnShaderCompileError;

EventManager::EventManager()
{
	OnShaderCompileError += NewDelegate(this, &EventManager::ShaderCompileError);
}

EventManager::~EventManager()
{
}

void EventManager::ShaderCompileError(int a)
{
	DebugLog("Shader compiled error,program has stopped.");
	cout << "\a" << endl;
	system("pause");

	exit(0);//����0ִ��������˳����򣺵��ø�����ֹ������򣬹ر�����I/O��.����1ֱ�ӽ����ں� ɶ��˼
	//return 0;
}

int EventManager::PathError(int a)
{
	return 0;
}
