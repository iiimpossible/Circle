#include "Timer.h"
#include <GLFW/glfw3.h>
#include "../Common.h"
Timer::Timer()
{
	this->bIsTiming = false;	
}

Timer::~Timer()
{

}

/* 如果每帧的时间大于设定的时间间隔，那么就来不及判定是否超出时间，这个时候时间间隔就变成了每帧时间*/
void Timer::SetTime(float aIntervel)
{
	if (!bIsTiming)
	{
		
		this->mStartTime = glfwGetTime();
#ifdef DEBUGLOG
		DebugData("@Timer::SetTime: Start Collision Time is: ", mStartTime);
#endif // DEBUGLOG
		this->mInterval = aIntervel;
		bIsTiming = true;
	}
	
}

bool Timer::IsTimeOut()
{
	if (bIsTiming)
	{
		float CurrentInterval = glfwGetTime() - mStartTime;
		if (CurrentInterval >= mInterval)
		{
#ifdef DEBUGLOG
			DebugData("@Timer::IsTimeOut: End Collision Time is: ", glfwGetTime());
#endif // DEBUGLOG			
			bIsTiming = false;
			return true;
		}
	 
	}
	return false;	
	
}
