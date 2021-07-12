#pragma once
#include <Windows.h>
#include "framework.h"

#ifdef CELIB_EXPORT
#define CELIB_API  extern "C" _declspec(dllexport)
#define CELIB_CLASS_API _declspec(dllexport)
#else
#define CELIB_API  extern "C" _declspec(dllimport)
#define CELIB_CLASS_API _declspec(dllimport)
#endif // CELIB_API

class Engine;

//启动引擎
CELIB_API Engine* InitCircleEngine();





