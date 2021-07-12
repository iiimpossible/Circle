#pragma once
 /**
 * @brief ���������Ϸ��ʱ��
 * @details �����һЩʱ����������趨Ϊ��̬�ģ�ͨ�������������������ʱ��һЩʱ��
 * ��Ϣ������ʱ����ϢҲ��Ҫ��ʼ���ģ���ô��ʼ���������Ϣ�أ�ʹ�ù��캯����ʼ����
 * ʱ����Ϣ��ʵ�Ǹ���glGetTime()����������ģ�ֻҪ�������
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
	 * @brief �ú���ͨ��
	 */
	static void CaculateDeltatime();
	static void CaculateAverageDeletatime();
	
private:
	///pre-deltatime
	 
	static float mDeltaTime;//�任�ļ��㵱ǰ֡��ʼ��ʱ���ȥ��һ֡��ʼ��ʱ�䣬ʵ��������һ֡��ʱ��
	static float mLastFrameTime;//��һ֡��ʼʱ��
	static float mCurrentFrameTime;//��ǰ֡��ʼʱ��
	static float mSleepTime;//�仯�ģ�������һ֡ʱ�䶯̬���㣬�����趨��֡ʱ���ȥ��һ֡��ʱ��
	static float mFPS;//ÿ֡ʵ��֡��
	static unsigned  mFPSCounter; // ֡������
	static float mOneSecond;//ʱ���
	static float mFPSUpdateInterval;// Ԥ��FPS����ʱ����

	static float mPreFPS;
};

