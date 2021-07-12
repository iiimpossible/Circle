
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
//�����Ǽ����굱ǰ֡ʱ�������ʱ��֮��û�м������ߵ�ʱ�䵼��֡��ʾ��׼
void CeTimer::CaculateDeltatime()
{
	mSleepTime = 0;

	mCurrentFrameTime = glfwGetTime();///<��ǰ֡ʱ��

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
	mCurrentFrameTime = glfwGetTime();///<��ǰ֡ʱ��
	mFPSCounter++;
	mOneSecond = mCurrentFrameTime - mLastFrameTime;//�ۼƹ�ȥһ�����ж���֡
	if (mOneSecond >= mFPSUpdateInterval)
	{
		//mSleepTime *=0.5f;
		mFPS = (mFPSCounter / mOneSecond );//����֡��
		//Ϊɶ����1����͵���֡��ֻ��60��5����Ҳ��60����������𣿿�����Sleep����������С��1ms�Ĳ���
		// 0.9  85
		/*time  counter  interval
		0		85		0.1
		0.9		85		0.1
		1.0		 7		0.1
		
		*/
		//DebugData("mOneSecond ", mOneSecond);
		//DebugData("mFPSCounter ", mFPSCounter);
		mFPSCounter = 0;//֡����������
		mLastFrameTime = mCurrentFrameTime;//��¼��ǰ֡�����ʱ��
		
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
