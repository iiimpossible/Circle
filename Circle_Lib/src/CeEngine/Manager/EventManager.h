#include <iostream>
#include "../../CCCommon/Tool/Delegate.h"

using namespace CircleEngine;


/**
 * @brief 引擎的事件管理类，目标是管理日志报错等，或许这个类应该是高于引擎底层（绘图、音频等基础模块），作为与用户
 * 界面交互的类，产生运行日志（控制台），并且不能将这个类嵌入到绘图API（着色器编译）中去，应该将该类调用放到Render类
 */
class EventManager
{
public:
	EventManager();

	~EventManager();	

	//需要改为动态定义事件吗？
	static Delegate<void,int> OnShaderCompileError;

	//Delegate<int,int> OnPathError;

private:

	void ShaderCompileError(int);

	int PathError(int);
	
	

};