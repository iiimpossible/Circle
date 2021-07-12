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

/* ���ÿ֡��ʱ������趨��ʱ��������ô���������ж��Ƿ񳬳�ʱ�䣬���ʱ��ʱ�����ͱ����ÿ֡ʱ��*/
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
