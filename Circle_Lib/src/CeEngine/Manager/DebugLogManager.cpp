#include "DebugLogManager.h"
#include "../Engine.h"



void DebugLogManager::DebugOutput(initializer_list<string> aList)
{
	string str;
	auto iter = aList.begin();
	while (iter != aList.end())
	{
		str += (*iter);
		iter++;		
	}
	str += '\n';
	Engine::mOnDebugLog.Invoke(str);//这个是引擎类的事件，供控制层使用，通知控制台输出信息
}



void DebugLogManager::DebugOutput(const string & aLocation, EDebugTag aTag, const vector<string>& aStrs)
{
	string str;
	for (auto val : aStrs)
	{
		str += val;
	}
	if (aTag == EDebugTag::EDT_ERROR)
	{
		DebugOutput({ "@",aLocation + " Error: ", str });
	}
	else if (aTag == EDebugTag::EDT_Log)
	{
		DebugOutput({ "@",aLocation + " Log: ", str });
	}
	else if (aTag == EDebugTag::EDT_Warning)
	{
		DebugOutput({ "@",aLocation + " Warning: ", str });
	}

}

void DebugLogManager::DebugOutput(const string& aLocation, EDebugTag aTag, const string & aHint, int aInfo)
{ 
	if (aTag == EDebugTag::EDT_ERROR)
	{
		DebugOutput({ "@" + aLocation + " Error: ", aHint, " ", TO_STR(aInfo) });
	}
	else if (aTag == EDebugTag::EDT_Log)
	{
		DebugOutput({ "@" + aLocation + " LOG: ", aHint, " ", TO_STR(aInfo) });
	}
	else if (aTag == EDebugTag::EDT_Warning)
	{
		DebugOutput({ "@" + aLocation + " Warning: ", aHint, " ", TO_STR(aInfo) });
	}	
}

void DebugLogManager::DebugOutput(const string& aLocation, EDebugTag aTag, const string & aHint, float aInfo)
{	
	if (aTag == EDebugTag::EDT_ERROR)
	{
		DebugOutput({ "@" + aLocation + " Error: ", aHint, " ", TO_STR(aInfo) });
	}
	else if (aTag == EDebugTag::EDT_Log)
	{
		DebugOutput({ "@" + aLocation + " LOG: ", aHint, " ", TO_STR(aInfo) });
	}
	else if (aTag == EDebugTag::EDT_Warning)
	{
		DebugOutput({ "@" + aLocation + " Warning: ", aHint, " ", TO_STR(aInfo) });
	}
}

void DebugLogManager::DebugOutputVec3(const string& aLocation, const Vec3& aVector)
{
	DebugOutput({ "@" + aLocation +" ", "x: ",TO_STR(aVector.x)," y: ",TO_STR(aVector.y)," z: ",TO_STR(aVector.z) });
}

void DebugLogManager::DebugOutputMat4(const string& aLocation, const Mat4& aMat)
{
	DebugOutput({ "@" + aLocation + " \n" });
	DebugOutput({ TO_STR(aMat[0][0])," ",TO_STR(aMat[1][0])," ",TO_STR(aMat[2][0])," ",TO_STR(aMat[3][0]) });
	DebugOutput({ TO_STR(aMat[0][1])," ",TO_STR(aMat[1][1])," ",TO_STR(aMat[2][1])," ",TO_STR(aMat[3][1]) });
	DebugOutput({ TO_STR(aMat[0][2])," ",TO_STR(aMat[1][2])," ",TO_STR(aMat[2][2])," ",TO_STR(aMat[3][2]) });
	DebugOutput({ TO_STR(aMat[0][3])," ",TO_STR(aMat[1][3])," ",TO_STR(aMat[2][3])," ",TO_STR(aMat[3][3]) });
}

void DebugLogManager::BindCallBack(const string & aStr)
{
	Engine::mOnDebugLog.Invoke(aStr);
}


