#pragma once
#include <string>
#include <vector>

class CeUtility
{
public:

	


public:
	//字符或字符串判断

	/** @breif 判断给定字符串是否属于关键字集合*/
	static bool IsKeywordMatch(const std::string& aKeyword, const std::vector<std::string> aKeySet);

	/** @breif 判断给定字符是否属于字符集合*/
	static bool IsCharMatch(char achar, const std::vector<char> aKeySet);

	/** @breif 判断目标字符串中是否含有模式串*/
	static bool FindStr(const std::string& aPattern,const std::string& aTarget ,int* aLoacation);

public:
	//字符或字符串操作

	/** @breif 将字符串中所有的某一字符替换为新的字符*/
	static void CharReplace(char aOldChar, char aNewChar, std::string& aTarget);

	/** @breif 从文件路径中获得文件名字*/
	static std::string GetFileNameFromPath(const std::string& aPath);

	/** @breif 从一个字符串中移除指定字符串*/
	static std::string RemoveWordFromStr(const std::string& aPattern, const std::string& aTarget);

	/** @breif 将文件路径拆分为后缀字符串和后缀之前的字符串*/
	static void SplitStringFromSuffix(const std::string& aStr, std::string& aFront, std::string& aSuffix);

public:
	//文件操作

	static void WriteText(const std::string& aPath, const std::string& aText);

	static std::string ReadText(const std::string& aPath);

};