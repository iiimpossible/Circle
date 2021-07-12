#pragma once
#include <iostream>
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
#include <Celib_x64/src/CeCommon/OpenGL/CeTextureFomatter.h>
using std::string;




 /**
 * @brief �����������ͣ��洢һ��2D������
 * @details ������������������ID
 */
class Texture2D 
{
public:	
	Texture2D(unsigned aTexHD = 0, int aWidth = 0, int aHeight = 0, int aChannels = 3);
	Texture2D(const Texture2DInfo& aInfo);
	//TODO:����ѡ����ˣ����Ʒ�ʽ��һ��ö��
	~Texture2D();
	
public:
	//������

	/** ���������*/
	inline unsigned GetTexture2DHandle() const { return mTex2DHD; }
	
public:
	//�������Ի�ȡ

	inline int GetWidth() const { return mWidth; };
	inline int GetHight() const { return mHeight; }
	inline int GetChannels() const { return mChannel; }

	inline const string& GetTypeName() { return mTypeName; }
	inline const string& GetPath() { return mPath; }
private:
	unsigned mTex2DHD;
	int mWidth; 
	int mHeight;
	int mChannel;
	string mTypeName;//��shader�й涨uniform�������������������texture_diffuse1����ʽ����
	string mPath;//���Ǽ��غ󣬴�assimp�л�õ�����·�����������¼��ص�����ȶԣ������ͬ����ζ�Ÿ������Ѿ�����
	//bool m_InternalFormat;
	//bool m_ImageFormat;
};
