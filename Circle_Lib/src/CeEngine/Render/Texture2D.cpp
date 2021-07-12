#include "Texture2D.h"


Texture2D::Texture2D(unsigned aTexHD, int aWidth, int aHeight, int aChannels):
	mTex2DHD(aTexHD), mWidth(aWidth), mHeight(aHeight), mChannel(aChannels)
{
	 
}

Texture2D::Texture2D(const Texture2DInfo& aInfo)
{
	mTex2DHD = aInfo.texHD;
	mWidth = aInfo.width;
	mHeight = aInfo.height;
	mChannel = aInfo.channel;
}



Texture2D::~Texture2D()
{

}

