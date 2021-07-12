#pragma once
#include <vector>
#include <string>
#include <map>
#include <initializer_list>
#include <CeCommon/Tool/CeStructs.h>
using std::vector;
using std::string;
using std::initializer_list;
using std::pair;


typedef pair<string, string> CompCmd;


/**
 * @breif 场景脚本词法器
 * @detail 从脚本中读取场景中所有的游戏物体的类型、名字、位置大小等信息。 *
 */
class SceneLexer
{
public:
	SceneLexer();
	~SceneLexer();

public:

	/** 函数：场景脚本词法解析器*/
	static int Lexer(const string& aStr, vector<ObjectInitInfo>* aInfo);

	/** 获得：调试日志*/
	static inline string GetDebugLog() { return mDebugLog; }

	static string WirteSceneScript(const string& aPath,const vector<ObjectInitInfo>& aInfo);

private:

	static bool IsNumber(char aChar);

	static bool IsLetter(char aChar);

	static bool IsSeparater(char aChar);

	static bool IsSkip(char aChar);

	static bool IsKeyword(const string& aWord);

	static bool IsOpenTag(const string& aTag);

	/** 比较：判断输入的关闭标签是否有对应的打开标签，栈的顶部元素*/
	static bool HasOpenTag(const string& aTag);


private:
	//字符串操作

	static string WriteOpenTag(const string& aStr);

	static string WriteCloseTag(const string& aStr);

	static string WriteObject(const string& aType, const string& aName);

	static string WriteComponent(const string& aType, const string& aCmd);


	static bool FindStr(const string& aPattern,const string& aTarget);

	static void CharReplace(char aOldChar, char aNewChar, string & aTaget);

	/** TODO:数字输入合法性检测。需要将数字提取出来一个一个字符进行语法分析
	目前如果输出的数字为0.0且与预期不符,那么是输入错错误*/
	static vector<float> TransformStrToFloat(const string& aStr);
	
	static void AddDebugLog(initializer_list<string> aList);
private:

	//关键字
	static vector<string> mKeyword;
	//标签
	static vector<string> mTag;
	//界符
	static vector<char> mSeparater;
	//跳过符（直接跳过/过滤的符号）空格、换行、回车、tab
	static vector<char> mSkip;
	//数字
	static vector<char> mNumber;
	//调试日志
	static string mDebugLog;
};

 