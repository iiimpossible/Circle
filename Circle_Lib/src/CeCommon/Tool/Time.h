#pragma once
 /**
 * @brief 该类管理游戏的时间
 * @details 这里的一些时间变量都被设定为静态的，通过该类来获得引擎运行时的一些时间
 * 信息。但是时间信息也是要初始化的，怎么初始化该类的信息呢？使用构造函数初始化吗？
 * 时间信息其实是根据glGetTime()这个函数来的，只要这个函数
 */
class CeTimer
{
public:
	static float GetFPS();
	static float GetDeltaTime();
	static float GetSleepTime();
	static float GetLastFrameTime();
	static float GetCurrentFrameTime();
	static float GetTime();
	
	static void SetFPS(float aFPS);
	/**
	 * @brief 该函数通过
	 */
	static void CaculateDeltatime();
	static void CaculateAverageDeletatime();
	
private:
	///pre-deltatime
	 
	static float mDeltaTime;//变换的计算当前帧开始的时间减去上一帧开始的时间，实际上是上一帧的时间
	static float mLastFrameTime;//上一帧开始时间
	static float mCurrentFrameTime;//当前帧开始时间
	static float mSleepTime;//变化的，根据上一帧时间动态计算，等于设定的帧时间减去上一帧的时间
	static float mFPS;//每帧实际帧数
	static unsigned  mFPSCounter; // 帧计数器
	static float mOneSecond;//时间段
	static float mFPSUpdateInterval;// 预设FPS更新时间间隔

	static float mPreFPS;
};

