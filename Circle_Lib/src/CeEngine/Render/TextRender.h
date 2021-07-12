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
* @brief 包含渲染位置，颜色，字体大小，文字间隔等
*/
struct TextLineParam
{
	string TextLine;
	Vec2 Position;
	float Scale;
	Vec3 Color; 	
};

/**
* @brief 这个类是字母对象
*/
struct Character
{
	unsigned TextureHandle;  // 字形纹理的ID
	Vec2 Size;       // 字形大小
	Vec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	unsigned Advance;    // 原点距下一个字形原点的距离
};
/**
* @brief 文字渲染
*/
class TextRender
{
public:
	TextRender();
	~TextRender();
	void Update();
	/**
	* @brief 渲染单行文本
	* 这个坐标原点在左下角，就是设备坐标系，因为没有是使用模型矩阵
	* @note 因为是使用纹理渲染的文字，所以字形放大倍数超过1.5就会
	* 感觉有点模糊了
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

 