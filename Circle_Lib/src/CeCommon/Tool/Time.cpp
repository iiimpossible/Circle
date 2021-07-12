
#include <GLFW/glfw3.h>
#include <algorithm>
#include "Time.h"
#include "../Common.h"

 
float CeTimer::mDeltaTime = 0.0f;
float CeTimer::mLastFrameTime = 0.0f;
float CeTimer::mCurrentFrameTime = 0.0f;
float CeTimer::mSleepTime = 0.0f;
float CeTimer::mFPS = 0.0f;
float CeTimer::mPreFPS = 60.0f;
float CeTimer::mOneSecond = 0.0f;
float CeTimer::mFPSUpdateInterval = 0.1f;
unsigned CeTimer::mFPSCounter = 0;
float CeTimer::GetFPS()
{
	//return 1.0f / mDeltaTime;
	return mFPS;
}

float CeTimer::GetDeltaTime()
{
	//return mDeltaTime;
	return 1.0f / mFPS ;
}

float CeTimer::GetSleepTime()
{
	return mSleepTime;
}

float CeTimer::GetLastFrameTime()
{
	return mLastFrameTime;
}

float CeTimer::GetCurrentFrameTime()
{
	return mCurrentFrameTime;
}

float CeTimer::GetTime()
{
	return glfwGetTime();
}

void CeTimer::SetFPS(float aFPS)
{
	mFPS = aFPS;
}
//可能是计算完当前帧时间和休眠时间之后，没有加上休眠的时间导致帧显示不准
void CeTimer::CaculateDeltatime()
{
	mSleepTime = 0;

	mCurrentFrameTime = glfwGetTime();///<当前帧时间

	mDeltaTime = mCurrentFrameTime - mLastFrameTime;

	mLastFrameTime = mCurrentFrameTime;

	float FPSTime = 1.0f / mFPS;
	if (mDeltaTime <= FPSTime)
	{
		mSleepTime = std::max((FPSTime - mDeltaTime) * 1000.0f, 0.0f);		
	}	
	//DebugData("deltaTime", mDeltaTime);
	//DebugData("FPSTime: ", FPSTime);
//	DebugData("mDeltaTime: ", mDeltaTime);
	//DebugData("mSleepTime: ", mSleepTime);
}

void CeTimer::CaculateAverageDeletatime()
{
	mCurrentFrameTime = glfwGetTime();///<当前帧时间
	mFPSCounter++;
	mOneSecond = mCurrentFrameTime - mLastFrameTime;//累计过去一秒内有多少帧
	if (mOneSecond >= mFPSUpdateInterval)
	{
		//mSleepTime *=0.5f;
		mFPS = (mFPSCounter / mOneSecond );//计算帧率
		//为啥挂起1毫秒就导致帧率只有60？5毫秒也是60？是算错了吗？可能是Sleep函数不接受小于1ms的参数
		// 0.9  85
		/*time  counter  interval
		0		85		0.1
		0.9		85		0.1
		1.0		 7		0.1
		
		*/
		//DebugData("mOneSecond ", mOneSecond);
		//DebugData("mFPSCounter ", mFPSCounter);
		mFPSCounter = 0;//帧计数器归零
		mLastFrameTime = mCurrentFrameTime;//记录当前帧的完成时间
		
		//DebugData("1.0f / mFPS ", 1.0f / mFPS);
		/*if ( (1.0f / mFPS)< (1.0 / mPreFPS))
		{
			mSleepTime = std::max( (float)((1.0 / mPreFPS) - (1.0f / mFPS))  , 0.0f);
			DebugData("mSleepTime ", mSleepTime);
		}*/
	/*	DebugData("1.0f / mFPS ", 1.0f / mFPS);
		if ((1.0f / mFPS) < 0.01667)
		{
			mSleepTime = std::max((float)(0.01667 - (1.0 / mFPS)), 0.0f);
			DebugData("mSleepTime ", mSleepTime);
		}*/
	}

}
