#include "SceneLexer.h"
#include <CeCommon/Tool/CeUtility.h>



vector<string> SceneLexer::mKeyword = { "type","name", };//transform �ؼ����Ѿ�ȥ��
vector<string> SceneLexer::mTag;
vector<char> SceneLexer::mSeparater = { '[',']','.' ,'<','>' };
vector<char> SceneLexer::mSkip = { ' ','\n','\t','\r' };//�ո� ���� �Ʊ�� �س�
vector<char> SceneLexer::mNumber = { '1','2','3','4','5','6','7','8','9','.', '-'};
string  SceneLexer::mDebugLog;
SceneLexer::SceneLexer(){}

SceneLexer::~SceneLexer(){}

int SceneLexer::Lexer(const string & aStr, vector<ObjectInitInfo>* aInfo)
{
	//ָ��
	int pointer = 0;
	//�м�����
	int line = 0;
	//ջ
	vector<string> stack;
	//��ʱ�洢�ؼ���
	string keyword;
	//��ʱ��������Ϣ
	ObjectInitInfo temp_info;

	vector<CompCmd> temp_comp_cmd;//��ʱ��������ǰ��������������������
	mDebugLog.clear();
	mTag.clear();
	while (pointer < aStr.size())
	{
		if (IsSkip(aStr[pointer]))//���1 �Ƿ�������
		{
			if (aStr[pointer] == '\n')
			{
				line++;
			}
			pointer++;
			continue;
		}
		else if (IsSeparater(aStr[pointer]))//���2 �Ƿ������жϹؼ���
		{
			if (aStr[pointer] == '[')//��Ϣ��ȡ��Ŀǰ�����ɫ���� �з����Żᵼ����Ϣ��ȡ��ȫ
			{
				pointer++;
				while (aStr[pointer] != ']')//ѭ��ʶ����ĸ
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}
				if (stack.back() == "type")
				{
					AddDebugLog({ "Message: ", keyword });
					temp_info.type = keyword;
					stack.pop_back();
					keyword.clear();
					continue;
				}
				else if (stack.back() == "name")
				{
					AddDebugLog({"Message: ", keyword});
					temp_info.name = keyword;
					stack.pop_back();
					keyword.clear();
					continue;
				}
			/*	else if (stack.back() == "transform")
				{
					AddDebugLog({ "Message: ", keyword });
					temp_info.transform = TransformStrToFloat(keyword);
					stack.pop_back();
					keyword.clear();
					continue;
				}*/
				else if (stack.back() == temp_comp_cmd.back().first)
				{
					AddDebugLog({ "Message: ", keyword });
					//string comp_type = stack.back()				 
					temp_comp_cmd.back().second = keyword;
					stack.pop_back();
					keyword.clear();
					continue;
				}
			 
				else
				{
					AddDebugLog({"Error: Keyword not exit in stack."});
					return 0;
				}
			}
			else if (aStr[pointer] == '<')//�ж��Ƿ���<>��ǵĹؼ��֣���������������
			{
				pointer++;
				while (aStr[pointer] != '>')
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}
				if (IsOpenTag(keyword))//�ж��Ƿ��ǿ�ʼ��ǩ
				{
					AddDebugLog({ "Log: Enter scope. ", keyword });
					mTag.push_back(keyword);
					keyword.clear();
					continue;
				}
				else if (HasOpenTag (keyword))//��mtagջ���Ƿ��ж�Ӧ�Ŀ�ʼ��ǩ
				{
					if (keyword == "/object")
					{						
						AddDebugLog({ "Log: End of scope. ", keyword });
						//����ȡ����Ϣ����������
					/*	if (temp_info.transform.size() != 9)
						{
							AddDebugLog({ "Error: Transfrom size invalid. Size =", std::to_string(temp_info.transform.size()) });
							return 0;
						}*/
						aInfo->push_back(temp_info);
						mTag.pop_back();
						keyword.clear();
						continue;						
					}
					else if (keyword == "/scene")
					{
						AddDebugLog({ "Log: End of scope. ", keyword });
						mTag.pop_back();
						keyword.clear();
						continue;
					}
					else if(keyword == "/component")
					{
						temp_info.comp_command = temp_comp_cmd;
						temp_comp_cmd.clear();
						mTag.pop_back();
						keyword.clear();
						continue;
					}
				}			
				else
				{
					AddDebugLog({ "Error: Scope exeption. ", keyword });
					keyword.clear();
					return 0;
				}
			}
			else if (aStr[pointer] == ']')
			{
				pointer++;
				continue;
			}
			else if (aStr[pointer] == '>')
			{
				pointer++;
				continue;
			}
		}
		else if (IsLetter(aStr[pointer]))//���3 letter �ж��ؼ���
		{
			//ѭ��ʶ��
			while (!IsSkip((aStr[pointer])) && !IsSeparater(aStr[pointer]))//��ĸ��ͷ�ĺ��һ����[ ���һ�����ַ�������ɫ����
			{
				keyword.push_back(aStr[pointer]);
				pointer++;
			}
			if (keyword == "type")
			{
				AddDebugLog({ "Keyword: ", "type" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "name")
			{
				AddDebugLog({ "Keyword: ", "name" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
		/*	else if (keyword == "transform")
			{
				AddDebugLog({ "Keyword: ", "transform" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}*/
			else if (FindStr("class",keyword))//Ȩ��֮�ƣ������ж��Ƿ������������
			{
				CharReplace('_', ' ', keyword);//�Ƚ�����ߵ��»����滻Ϊ�ո���ΪC++��typeinfo�õ��Ķ�̬������Ϊ��class type��
				AddDebugLog({ "Keyword: ",  keyword });
				temp_comp_cmd.push_back(CompCmd(keyword,""));//��ʱ������
				stack.push_back(keyword);//���ؼ��ִ���ջ��
				keyword.clear();
				continue;
			}
			else
			{
				AddDebugLog({ "Error: ", "Invalid keyword.", "@line ", std::to_string(line), " ", std::to_string(aStr[pointer]), " ", keyword });
				keyword.clear();
				return 0;
			}
		}
		else//���4 �� ��
		{
			AddDebugLog({ "Error: ", "Invalid input.", "@line ", std::to_string(line), " ", std::to_string(aStr[pointer]), " ", keyword });
			return 0;
		}
		//���5 ����
		//����
	}
	return 1;
}

string SceneLexer::WirteSceneScript(const string& aPath, const vector<ObjectInitInfo>& aInfo)
{
	string scene_str;
	scene_str += WriteOpenTag("scene");
	//д��Ϸ�������
	for (int i = 0; i < aInfo.size(); i++)
	{
		scene_str.push_back('\t');
		scene_str += WriteOpenTag("object");
		//д��Ϸ�������ͺ�����
		scene_str.push_back('\t');
		scene_str += WriteObject(aInfo[i].type, aInfo[i].name);

		scene_str.push_back('\t');
		scene_str.push_back('\t');
		scene_str += WriteOpenTag("component");
		//д���

		for (auto comp : aInfo[i].comp_command)
		{
			scene_str.push_back('\t');
			scene_str.push_back('\t');
			scene_str += WriteComponent(comp.first,comp.second);
		}
		scene_str.push_back('\t');
		scene_str.push_back('\t');
		scene_str += WriteCloseTag("component");
		scene_str.push_back('\t');
		scene_str += WriteCloseTag("object");
		//һ����Ϸ���������
		scene_str.push_back('\n');
		scene_str.push_back('\n');
	}	
	scene_str += WriteCloseTag("scene");
	//����д�뺯��

	mDebugLog.clear();
	AddDebugLog({scene_str});
	
	return scene_str;
}

bool SceneLexer::IsNumber(char aChar)
{
	if (aChar >= '-' && aChar <= '9') return true;
	return false;
}

bool SceneLexer::IsLetter(char aChar)
{
	if (aChar >= 'A' && aChar <= 'Z') return true;
	if (aChar >= 'a' && aChar <= 'z') return true;
	return false;
}

bool SceneLexer::IsSeparater(char aChar)
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

bool SceneLexer::IsSkip(char aChar)
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

bool SceneLexer::IsKeyword(const string & aWord)
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

bool SceneLexer::IsOpenTag(const string & aTag)
{
	if (!aTag.empty())
	{
		if (IsLetter(aTag[0]))
			return true;
	}
	return false;
}

bool SceneLexer::HasOpenTag(const string & aTag)
{
	if (aTag.empty())return false;
	string temp_str(aTag,1);
	if (!mTag.empty())
	{
		if (mTag.back() == temp_str)
			return true;	
	} 
	return false;
}

string SceneLexer::WriteOpenTag(const string & aStr)
{
	string temp = '<' + aStr + '>' + '\n';
	return temp;
}

string SceneLexer::WriteCloseTag(const string & aStr)
{
	string temp = "</" + aStr + '>' + '\n';
	return temp;
}

string SceneLexer::WriteObject(const string & aType, const string & aName)
{
	string temp;
	string type = CeUtility::RemoveWordFromStr("class ",aType);
	temp += "type";
	temp.push_back('[');
	temp += type;
	temp.push_back(']');
	temp.push_back('\n');

	temp.push_back('\t');
	temp += "name";
	temp.push_back('[');
	temp += aName;
	temp.push_back(']');
	temp.push_back('\n');
	 
	return temp;
}

string SceneLexer::WriteComponent(const string & aType, const string & aCmd)
{
	string temp;
	string type = aType;
	CeUtility::CharReplace(' ','_',type);

	temp += type;
	temp.push_back('[');
	temp += aCmd;
	temp.push_back(']');
	temp.push_back('\n');
	 
	return temp;
}

bool SceneLexer::FindStr(const string & aPattern, const string & aTarget)
{
	int stride = aPattern.size();
	if (stride > aTarget.size()) return false;
	string fragment;
	for(int i =0;(i+stride) < aTarget.size();i++)
	{
		fragment = aTarget.substr(i,stride);
		if (fragment == aPattern)
		{
			return true;
		}
	} 
	return false;
}

void SceneLexer::CharReplace(char aOld,char aNew, string & aTaget)
{
	for (int i = 0; i < aTaget.size(); i++)
	{
		if (aTaget[i] == aOld)
		{
			aTaget[i] = aNew;
		}
	}
 
}

vector<float> SceneLexer::TransformStrToFloat(const string & aStr)
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
				AddDebugLog({"Error: ", "Invalid transform, first char must be a number. ", std::to_string(aStr[pointer])});
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
	return nums;
}

void SceneLexer::AddDebugLog(initializer_list<string> aList)
{
	for (auto val : aList)
	{
		mDebugLog+=val;
	}
	mDebugLog.push_back('\n');
}
