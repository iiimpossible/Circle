#include "CeVM.h"

vector<string> CeVM::mKeyword{"Vec3"};
//���
vector<char> CeVM::mSeparater = { '(',')','.'};
//��������ֱ������/���˵ķ��ţ��ո񡢻��С��س���tab
vector<char> CeVM::mSkip = { ' ','\n','\t','\r',',' };//�ո� ���� �Ʊ�� �س�
//����
vector<char> CeVM::mNumber = { '1','2','3','4','5','6','7','8','9','.', '-' };

string CeVM::mDebugLog;


string CeVM::Interpreter(EInstruction aIns, const string & aCommand)
{

	switch (aIns)
	{
	case Set_Vec3:

		break;
	case Set_Vec2:
		break;
	case Set_Mat4:
		break;
	default:
		break;
	}

	return string();
}

Vec3 CeVM::StrToVec3(const string & aCommand)
{
	//string str = RemoveBracket(aCommand);
	auto temp_val = StrToFloat(aCommand, 3);

	if (temp_val.size() == 3)
	{
		return Vec3(temp_val[0], temp_val[1], temp_val[2]);
	}
	else
	{
		return Vec3(0.0f);
	}
}

vector<Vec3> CeVM::StrToTransform(const string & aCommand)
{
	vector<Vec3> temp_trans;
	vector<float> val = StrToFloat(aCommand,9);
	 temp_trans.emplace_back(val[0], val[1], val[2]);
	 temp_trans.emplace_back(val[3], val[4], val[5]);
	 temp_trans.emplace_back(val[6], val[7], val[8]);
	return temp_trans;
}



/**
 * @breif ָ����Ͳ�ͬ�ڳ����ű���ָ��ͨ���ж������
 * Set_Vec3 (1.0,2.0,3.0) ��Ԫʽ
 * ָ���ʽ��һ��ʽ�ؼ���+��������
 */
string CeVM::CeVMLexer(const string & aStr)
{
	vector<float> trans = StrToFloat(aStr,9);
	return string();
}



bool CeVM::IsNumber(char aChar)
{
	if (aChar >= '-' && aChar <= '9') return true;
	return false;
}

bool CeVM::IsLetter(char aChar)
{
	if (aChar >= 'A' && aChar <= 'Z') return true;
	if (aChar >= 'a' && aChar <= 'z') return true;
	return false;
}

bool CeVM::IsSeparater(char aChar)
{
	for (auto val : mSeparater)
	{
		if (val == aChar)
		{
			return true;
		}
	}
	return false;
}

bool CeVM::IsSkip(char aChar)
{
	for (auto val : mSkip)
	{
		if (val == aChar)
		{
			return true;
		}
	}
	return false;
}

bool CeVM::IsKeyword(const string & aWord)
{
	for (auto val : mKeyword)
	{
		if (val == aWord)
		{
			return true;
		}
	}
	return false;
}

void CeVM::AddDebugLog(initializer_list<string> aList)
{
	for (auto val : aList)
	{
		mDebugLog += val;
	}
	mDebugLog.push_back('\n');
}

vector<float> CeVM::StrToFloat(const string & aStr, const unsigned aDemension)
{
	int pointer = 0;

	vector<float> nums;

	string str_num;

	while (pointer < aStr.size())
	{
		if (IsNumber(aStr[pointer]))//1.����״̬
		{
			while (IsNumber(aStr[pointer]) || aStr[pointer] == '.' || aStr[pointer] == '-')
			{
				str_num.push_back(aStr[pointer]);
				pointer++;
			}
			float f = atof(str_num.c_str());
			str_num.clear();
			nums.push_back(f);
			continue;
			//TODO:��������Ϸ��Լ�⡣��Ҫ��������ȡ����һ��һ���ַ������﷨����
			//Ŀǰ������������Ϊ0.0����Ԥ�ڲ���,��ô����������
		}
		else if (IsSkip(aStr[pointer]))//2.������״̬
		{
			if (pointer == 0)
			{
				AddDebugLog({ "Error: ", "Invalid transform, first char must be a number. ", std::to_string(aStr[pointer]) });
				return {};
			}
			else
			{
				pointer++;
				continue;
			}
		}
		else
		{
			AddDebugLog({ "Error: ", "Invalid input. ", std::to_string(aStr[pointer]) });
			return {};
		}
	}


	if (nums.size() != aDemension)
	{
		AddDebugLog({ "Error: ", "Invalid input, demension error. ", std::to_string(aStr[pointer]) });
		vector<float> error(aDemension);
		return  error;
	}

	return nums;
}

string CeVM::RemoveBracket(const string & aStr)
{
	//string bracket_stack;
	int pointer = 0;
	string keyword;
	while (pointer != aStr.size())
	{
		if (aStr[pointer] == '(')
		{
			pointer++;
			while (aStr[pointer]!= ')' )
			{
				keyword.push_back(aStr[pointer]);
				pointer++;
			}			
		}
		else if (aStr[pointer] == ')')
		{
			continue;
		}
		else if (aStr[pointer] == ' ')
		{
			pointer++;
			continue;
		}
		pointer++;
	}
	return keyword;
}
 

 