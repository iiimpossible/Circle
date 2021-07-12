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

	exit(0);//参数0执行清理后退出程序：调用各种终止处理程序，关闭所有I/O流.参数1直接进入内核 啥意思
	//return 0;
}

int EventManager::PathError(int a)
{
	return 0;
}
