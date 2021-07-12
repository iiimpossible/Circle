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

/** ��ɫ��ԭ��*/
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

	/** �ɽӿڣ�һ������ֻ����һ����ɫ�����޶�����*/
	static void LexicalAnalysis(const string& aStr, vector<string>* aName,vector<string>* aPath, ShaderCode* aCode);
	static void LexicalAnalysis(const string& aStr);

	/** 
	 * @brief ���ж����ǵĲ��ʽű�������
	 * @detail �½ӿڣ�һ�����ʶ�������ɫ���Ҿ����ض�������	�ж����ǡ�
 	 * ���ڶ������ģ�ͣ�һ��������Ҫһ����ɫ�������Ҹ�������Ҫ��Ӧ������
	 * ��ͬ���������һ�������ֻʹ��һ����ɫ�����ᵼ������ӳ�����
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

	/** �Ƚϣ��ж�����Ĺرձ�ǩ�Ƿ��ж�Ӧ�Ĵ򿪱�ǩ��ջ�Ķ���Ԫ��*/
	static bool HasOpenTag(const string& aTag, const vector<string>& aTags);
	static vector<float> TransformStrToFloat(const string& aStr);
private:
	static string mDebugLog;

	static vector<string> mKeyWord;
	//#�ǹؼ��ֵı�־��$����ɫ���ı�־��[]��·����־
	static vector<char> mSaperater;

	static vector<char> mSkip;//�ո� ���� �Ʊ�� �س�

	static vector<string> mSymbolTable;

	
};



