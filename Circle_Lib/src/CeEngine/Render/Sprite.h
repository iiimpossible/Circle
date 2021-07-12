#pragma once
#include "Shader.h"
#include "Texture2D.h"


//用于2D游戏渲染的材质信息，与Material同一级别，不同的是该类用于2D渲染
class Sprite
{
public:

	void SetShader(const Shader& aShader);
	void SetTexture2D(const Texture2D& aTex2D);

	/**
	 * @brief 设置序列帧动画的行列，如果不调用该方法，那么就不会使用序列帧动画的着色器
	 */
	void SetRank(float aRow, float aCloum);

	void SetTimeSpeed(float aSpeed = 20.0f);
	//const修饰成员函数，哪个该函数不能修改成员的指，
	//const修饰指针或引用返回值，那么该返回值不能改变
	const Texture2D& GetTexture2D() const;
	const Shader& GetShader() const;
private:	
	Shader mShader;
	Texture2D mTexture2D;	
	
};