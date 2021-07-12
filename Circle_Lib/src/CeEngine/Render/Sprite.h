#pragma once
#include "Shader.h"
#include "Texture2D.h"


//����2D��Ϸ��Ⱦ�Ĳ�����Ϣ����Materialͬһ���𣬲�ͬ���Ǹ�������2D��Ⱦ
class Sprite
{
public:

	void SetShader(const Shader& aShader);
	void SetTexture2D(const Texture2D& aTex2D);

	/**
	 * @brief ��������֡���������У���������ø÷�������ô�Ͳ���ʹ������֡��������ɫ��
	 */
	void SetRank(float aRow, float aCloum);

	void SetTimeSpeed(float aSpeed = 20.0f);
	//const���γ�Ա�������ĸ��ú��������޸ĳ�Ա��ָ��
	//const����ָ������÷���ֵ����ô�÷���ֵ���ܸı�
	const Texture2D& GetTexture2D() const;
	const Shader& GetShader() const;
private:	
	Shader mShader;
	Texture2D mTexture2D;	
	
};