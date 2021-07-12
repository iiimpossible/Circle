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
 * @breif �����ű��ʷ���
 * @detail �ӽű��ж�ȡ���������е���Ϸ��������͡����֡�λ�ô�С����Ϣ�� *
 */
class SceneLexer
{
public:
	SceneLexer();
	~SceneLexer();

public:

	/** �����������ű��ʷ�������*/
	static int Lexer(const string& aStr, vector<ObjectInitInfo>* aInfo);

	/** ��ã�������־*/
	static inline string GetDebugLog() { return mDebugLog; }

	static string WirteSceneScript(const string& aPath,const vector<ObjectInitInfo>& aInfo);

private:

	static bool IsNumber(char aChar);

	static bool IsLetter(char aChar);

	static bool IsSeparater(char aChar);

	static bool IsSkip(char aChar);

	static bool IsKeyword(const string& aWord);

	static bool IsOpenTag(const string& aTag);

	/** �Ƚϣ��ж�����Ĺرձ�ǩ�Ƿ��ж�Ӧ�Ĵ򿪱�ǩ��ջ�Ķ���Ԫ��*/
	static bool HasOpenTag(const string& aTag);


private:
	//�ַ�������

	static string WriteOpenTag(const string& aStr);

	static string WriteCloseTag(const string& aStr);

	static string WriteObject(const string& aType, const string& aName);

	static string WriteComponent(const string& aType, const string& aCmd);


	static bool FindStr(const string& aPattern,const string& aTarget);

	static void CharReplace(char aOldChar, char aNewChar, string & aTaget);

	/** TODO:��������Ϸ��Լ�⡣��Ҫ��������ȡ����һ��һ���ַ������﷨����
	Ŀǰ������������Ϊ0.0����Ԥ�ڲ���,��ô����������*/
	static vector<float> TransformStrToFloat(const string& aStr);
	
	static void AddDebugLog(initializer_list<string> aList);
private:

	//�ؼ���
	static vector<string> mKeyword;
	//��ǩ
	static vector<string> mTag;
	//���
	static vector<char> mSeparater;
	//��������ֱ������/���˵ķ��ţ��ո񡢻��С��س���tab
	static vector<char> mSkip;
	//����
	static vector<char> mNumber;
	//������־
	static string mDebugLog;
};

 