#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include <Celib_x64/src/CeCommon/Common.h>

using std::vector;
using std::string;
using std::initializer_list;
using namespace CircleEngine;

#define TO_STR(str) std::to_string(str)

enum EDebugTag
{
	EDT_Log = 1,
	EDT_ERROR = 2,
	EDT_Warning = 3
};

//#define CE_Log(...)
/**
 * @brief ������һ������־���������̨���࣬����һЩ��̬����
 */
class DebugLogManager
{

public:
	//���޲���
	static void DebugOutput(initializer_list<string> aList);

	/* ����ģ���������*/
	template<typename T>
	static void DebugOutput(EDebugTag aTag, initializer_list<string> aList)
	{
		string tStr;
		string type_name = typeid(T).name();
		if (aTag == EDebugTag::EDT_ERROR)
		{
			tStr = "@" + type_name + " ERROR: ";
		}
		else if (aTag == EDebugTag::EDT_Log)
		{
			tStr = "@" + type_name + " Log: ";
		}
		else if (aTag == EDebugTag::EDT_Warning)
		{
			tStr = "@" + type_name + " Warning: ";
		}
		auto iter = aList.begin();
		while (iter != aList.end())
		{
			tStr += (*iter);
			iter++;
		}
		BindCallBack(tStr);//�������������¼��������Ʋ�ʹ�ã�֪ͨ����̨�����Ϣ
	}

	/* �����������*/
	static void DebugOutput(const string& aLocation, EDebugTag aTag, const vector<string>& aStrs);

	static void DebugOutput(const string& aLocation, EDebugTag aTag, const string& aHint, int aInfo);
	static void DebugOutput(const string& aLocation, EDebugTag aTag, const string& aHint, float aInfo);
	static void DebugOutputVec3(const string& aLocation, const Vec3& aVector);
	static void DebugOutputMat4(const string& aLocation, const Mat4& aMat);

//public:
//	//������
//	static void DebugLog(const string& aLog);
//
//	static void DebugLog(int aParam);
//
//	static void DebugLog(float aParam);
//
//public:
//	//��������
//	static void DebugLog(const string& aLog, int aParam);
//	static void DebugLog(const string& aLog, float aParam);
//public:
//	//��������
//	static void DebugLog(const string& aLog1, int aParam, const string& aLog2);

public:
	//�¼�
	
	
private:

	static void IntToString();

	static void FloatToString();

	static void BindCallBack(const string& aStr);

};