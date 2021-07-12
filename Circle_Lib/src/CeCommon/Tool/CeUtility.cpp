#include "CeUtility.h"
#include <math.h>
#include <algorithm>
using namespace std;
bool CeUtility::IsKeywordMatch(const std::string & aKeyword, const std::vector<std::string> aKeySet)
{
	return false;
}

bool CeUtility::IsCharMatch(char achar, const std::vector<char> aKeySet)
{
	return false;
}

bool CeUtility::FindStr(const std::string & aPattern, const std::string & aTarget, int* aLoacation)
{
	int stride = aPattern.size();
	if (stride > aTarget.size()) return false;
	std::string fragment;
	for (int i = 0; (i + stride) < aTarget.size(); i++)
	{
		fragment = aTarget.substr(i, stride);
		if (fragment == aPattern)
		{
			*aLoacation = i;
			return true;
		}
	}
	*aLoacation = 0;
	return false;
}

void CeUtility::CharReplace(char aOldChar, char aNewChar, std::string & aTarget)
{
	for (int i = 0; i < aTarget.size(); i++)
	{
		if (aTarget[i] == aOldChar)
		{
			aTarget[i] = aNewChar;
		}
	}

}

std::string CeUtility::GetFileNameFromPath(const std::string & aPath)
{
	//循环
	std::string str;
	for (auto val : aPath)
	{
		str.push_back(val);
		if ((val == '\\') || (val == '/'))
		{
			str.clear();
		}
	}
	return str;
}

std::string CeUtility::RemoveWordFromStr(const std::string & aPattern, const std::string & aTarget)
{
	std::string front;
	std::string back;
	int loacation = 0;
	if (FindStr(aPattern, aTarget, &loacation))
	{		
		front = aTarget.substr(0, max(loacation - 1, 0));//(0,0)合法吗？

		back = aTarget.substr(min((int)aTarget.size() - 1, loacation + (int)aPattern.size()));;
		return front + back;
	} 
	else
	{
		return {};
	}
	
}

void CeUtility::SplitStringFromSuffix(const std::string& aStr, std::string & aFront, std::string & aSuffix)
{
	//asset/material/shader/shader.shader

	//首先过一遍检查'.'的数量
	int num = 0;
	int location = 0;
	for (size_t i = 0; i < aStr.size(); i++)
	{
		if (aStr[i] == '.')
		{
			num++;
			location = i;	
		}
	}
	if (num != 1)return;
	aFront = aStr.substr(0, max(location, 0));//(0,0)合法吗？
	aSuffix = aStr.substr(min(location, (int)aStr.size()));
}

#include <iostream>
#include <fstream>
void CeUtility::WriteText(const std::string& aPath, const std::string & aText)
{
	std::ofstream ofile;
	ofile.open(aPath);
	ofile << aText<<std::endl;
	ofile.close();
}

std::string CeUtility::ReadText(const std::string & aPath)
{
	std::string text;
	std::ifstream ifile;
	ifile.open(aPath);
	ifile >> text;
	ifile.close();
	return text;
}
