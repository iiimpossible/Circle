#pragma once
#include "../../CCCommon/Common.h"
#include <string>
#include <vector>
#include <map>
using std::pair;
using std::map;
using std::vector;
using std::string;
using CircleEngine::Vec2;
using CircleEngine::Vec3;

/**
* @brief ������Ⱦλ�ã���ɫ�������С�����ּ����
*/
struct TextLineParam
{
	string TextLine;
	Vec2 Position;
	float Scale;
	Vec3 Color; 	
};

/**
* @brief ���������ĸ����
*/
struct Character
{
	unsigned TextureHandle;  // ���������ID
	Vec2 Size;       // ���δ�С
	Vec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	unsigned Advance;    // ԭ�����һ������ԭ��ľ���
};
/**
* @brief ������Ⱦ
*/
class TextRender
{
public:
	TextRender();
	~TextRender();
	void Update();
	/**
	* @brief ��Ⱦ�����ı�
	* �������ԭ�������½ǣ������豸����ϵ����Ϊû����ʹ��ģ�;���
	* @note ��Ϊ��ʹ��������Ⱦ�����֣��������ηŴ�������1.5�ͻ�
	* �о��е�ģ����
	*/
	/**,. */	 
	static void RenderTextLine(const string& TextLine,Vec2 Position = Vec2(0.0f), float aScale = 1.0f, Vec3 aColor =Vec3(1.0f));
	
private:
	static void DrawTextLine(const TextLineParam & aParam);
	static vector<TextLineParam> mTextLine;
	static unsigned mShaderHandle;
	static unsigned mVAO;
	static unsigned mVBO;

	
	static void InitFreeType();
	static map<char, Character> mCharacters;
};

 