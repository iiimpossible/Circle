// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include <CeCommon/OpenGL/CeWindow.h>
#include <CeEngine/Manager/InputManager.h>
#include <CeEngine/Manager/SceneManager.h>
#include <CeEngine/Manager/ScriptManager.h>
#include <CeEngine/Engine.h>


Engine * InitCircleEngine()
{
	return  new Engine();
}


 