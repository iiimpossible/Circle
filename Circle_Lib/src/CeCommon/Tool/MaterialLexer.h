#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
using std::string;
using std::vector;
using std::initializer_list;

#define Debug(...)\
vector<string>  str{__VA_ARGS__};\
for(auto val: str)\
{\
	mDebugLog += val;\
}\
mDebugLog.push_back('\n');


struct TextureLoadInfo
{
	string path;
	string name;
};

struct ShaderCode
{
	string vertex;
	string fragment;
	string geometry;
};

/** 着色器原形*/
struct ShaderPrototype
{
	vector<string> texture_name;
	vector<string> texture_path;	
	string vertex;
	string fragment;
	string geometry;

	inline void clear()
	{
		texture_name.clear();
		texture_path.clear();	
	}	
};

struct MaterialPrototype
{
	float color[3]{ 0.0f,0.0f,0.0f };
};

class CeMaterialLexer
{
public:
	CeMaterialLexer();
	~CeMaterialLexer();

	/** 旧接口，一个材质只编译一个着色器，无段落标记*/
	static void LexicalAnalysis(const string& aStr, vector<string>* aName,vector<string>* aPath, ShaderCode* aCode);
	static void LexicalAnalysis(const string& aStr);

	/** 
	 * @brief 具有段落标记的材质脚本解析器
	 * @detail 新接口，一个材质定义多个着色器且具有特定的纹理，	有段落标记。
 	 * 对于多网格的模型，一个网格需要一个着色器，并且该网格需要对应的纹理，
	 * 不同网格的纹理不一样。如果只使用一个着色器，会导致纹理映射错误
	 */
	static int LexicalAnalysis(const string& aStr, vector<ShaderPrototype>* aShader, MaterialPrototype* aMaterial);	
		 
	static inline string GetDebugLog() { return mDebugLog; }

	static inline void ClearLog() { mDebugLog.clear(); }


private:

	static bool IsNumber(char aChar);
	static bool IsSkip(const char aChar);
	static bool IsKeyWord(const string& aStr);
	static bool IsSaperater(const char aChar);
	static bool IsLetter(char aChar);

	static void AddDebugLog(initializer_list<string> aList);

	static bool IsOpenTag(const string& aTag);

	/** 比较：判断输入的关闭标签是否有对应的打开标签，栈的顶部元素*/
	static bool HasOpenTag(const string& aTag, const vector<string>& aTags);
	static vector<float> TransformStrToFloat(const string& aStr);
private:
	static string mDebugLog;

	static vector<string> mKeyWord;
	//#是关键字的标志，$是着色器的标志，[]是路径标志
	static vector<char> mSaperater;

	static vector<char> mSkip;//空格 换行 制表符 回车

	static vector<string> mSymbolTable;

	
};



