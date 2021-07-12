#pragma once
#include <string>
#include <vector>

class CeUtility
{
public:

	


public:
	//�ַ����ַ����ж�

	/** @breif �жϸ����ַ����Ƿ����ڹؼ��ּ���*/
	static bool IsKeywordMatch(const std::string& aKeyword, const std::vector<std::string> aKeySet);

	/** @breif �жϸ����ַ��Ƿ������ַ�����*/
	static bool IsCharMatch(char achar, const std::vector<char> aKeySet);

	/** @breif �ж�Ŀ���ַ������Ƿ���ģʽ��*/
	static bool FindStr(const std::string& aPattern,const std::string& aTarget ,int* aLoacation);

public:
	//�ַ����ַ�������

	/** @breif ���ַ��������е�ĳһ�ַ��滻Ϊ�µ��ַ�*/
	static void CharReplace(char aOldChar, char aNewChar, std::string& aTarget);

	/** @breif ���ļ�·���л���ļ�����*/
	static std::string GetFileNameFromPath(const std::string& aPath);

	/** @breif ��һ���ַ������Ƴ�ָ���ַ���*/
	static std::string RemoveWordFromStr(const std::string& aPattern, const std::string& aTarget);

	/** @breif ���ļ�·�����Ϊ��׺�ַ����ͺ�׺֮ǰ���ַ���*/
	static void SplitStringFromSuffix(const std::string& aStr, std::string& aFront, std::string& aSuffix);

public:
	//�ļ�����

	static void WriteText(const std::string& aPath, const std::string& aText);

	static std::string ReadText(const std::string& aPath);

};