#pragma once

class Timer
{
public:
	Timer();
	~Timer();

	void SetTime(float aIntervel);
	bool IsTimeOut();
private:
	float mStartTime;
	float mInterval;
	bool bIsTiming;

};

 