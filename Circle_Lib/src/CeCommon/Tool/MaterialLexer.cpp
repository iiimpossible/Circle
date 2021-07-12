#include "MaterialLexer.h"
#include "CeLoader.h"
using namespace std;

string CeMaterialLexer::mDebugLog;
vector<string> CeMaterialLexer::mKeyWord{"color","ambient", "diffuse", "specular","name","path","vertex","fragment","geometry" };
//#�ǹؼ��ֵı�־��$����ɫ���ı�־��[]��·����־
vector<char> CeMaterialLexer::mSaperater{ '#','[',']','<','>','"' };//TODO:Ϊ������ɫ�������е�[]��<>���֣�δ��vertex[]��[]��Ҫ���"
vector<char> CeMaterialLexer::mSkip{ ' ','\n','\t','\r' };//�ո� ���� �Ʊ�� �س�
vector<string> CeMaterialLexer::mSymbolTable;

CeMaterialLexer::CeMaterialLexer()
{
}

CeMaterialLexer::~CeMaterialLexer()
{
}

void CeMaterialLexer::LexicalAnalysis(const string& aStr, vector<string>* aName, vector<string>* aPath, ShaderCode* aCode)
{
	//�����ż�����	 
	int SqBracketCounter = 0;
	int pointer = 0;//�ı�ɨ��ָ��
	int line = 1;//�м�����
	string keyword;//�ؼ���
	vector<string>  stack;//���﷨ջ

	mDebugLog.clear();//�����֮ǰ����־

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
		else if (IsSaperater(aStr[pointer]))//���2 �Ƿ������жϹؼ���
		{
			if (aStr[pointer] == '#')//����#���һ�����ؼ���
			{
				pointer++;
				while (IsLetter(aStr[pointer]))//ѭ��ʶ����ĸ
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}

				if (keyword == "Texture")
				{
					AddDebugLog({ "Keyword: ", "Texture" });					 
					stack.push_back(keyword);
					keyword.clear();
					continue;
				}
				else if (keyword == "Shader")
				{
					AddDebugLog({ "Keyword: ", "Shader" });
					stack.push_back(keyword);
					keyword.clear();
					continue;
				}
				else
				{
					AddDebugLog({ "Error: ", "Invalid keyword after '#'", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
					stack.push_back(keyword);
					keyword.clear();
					return;
				}
			}
			else if (aStr[pointer] == '[')//��Ϣ��ȡ��Ŀǰ�����ɫ���� �з����Żᵼ����Ϣ��ȡ��ȫ
			{
				++SqBracketCounter;
				pointer++;
				for (; pointer < aStr.size(); pointer++)//ѭ��ʶ����ĸ
				{
					if(aStr[pointer] == '[') ++SqBracketCounter;
					if (aStr[pointer] == ']')
					{
						if (SqBracketCounter==1)
						{
							break;
						}
						else
						{
							--SqBracketCounter;
						}
					}					
					keyword.push_back(aStr[pointer]);					
				}
			 
				if (stack.back() == "name")
				{
					AddDebugLog({ "Message: ", keyword });
					aName->push_back(keyword);
					keyword.clear();
					continue;

				}
				else if (stack.back() == "path")
				{
					AddDebugLog({ "Message: ", keyword });
					aPath->push_back(keyword);
					keyword.clear();
					continue;
				}
				else if (stack.back() == "vertex")
				{
					aCode->vertex = keyword;
					keyword.clear();
					continue;
				}
				else if (stack.back() == "fragment")
				{
					aCode->fragment = keyword;
					keyword.clear();
					continue;
				}
				else if (stack.back() == "geometry")
				{
					aCode->geometry = keyword;
					keyword.clear();
					continue;
				}
			}
			else if (aStr[pointer] == ']')
			{
				--SqBracketCounter;
				pointer++;
				continue;
			}

		}
		else if (IsLetter(aStr[pointer]))//���3 letter �ж��ؼ���
		{
			//ѭ��ʶ��
			while (IsLetter(aStr[pointer]))//��ĸ��ͷ�ĺ��һ����[ ���һ�����ַ�������ɫ����
			{
				keyword.push_back(aStr[pointer]);
				pointer++;
			}
			if (keyword == "name")
			{
				AddDebugLog({ "Keyword: ", "name" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "path")
			{
				AddDebugLog({ "Keyword: ", "path" });				
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "vertex")
			{
				AddDebugLog({ "Keyword: ", "vertex" });				
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "fragment")
			{
				AddDebugLog({ "Keyword: ", "fragment" });				
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "geometry")
			{
				AddDebugLog({ "Keyword: ", "geometry" });				
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else
			{
				AddDebugLog({ "Error: ", "Invalid keyword.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });				
				keyword.clear();
				return;
			}
		}
		else//���4 �� ��
		{
			AddDebugLog({ "Error: ", "Invalid input.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
			return;
		}
		//���5 ����

		//����
	}
}

void CeMaterialLexer::LexicalAnalysis(const string & aStr)
{
	int pointer = 0;
	int line = 1;
	string keyword;
	vector<string>  stack;

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
		else if (IsSaperater(aStr[pointer]))//���2 �Ƿ���
		{
			if (aStr[pointer] == '#')//����#���һ�����ؼ���
			{
				pointer++;
				while (IsLetter(aStr[pointer]))//ѭ��ʶ����ĸ
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}

				if (keyword == "Texture")
				{
					AddDebugLog({ "Keyword: ", "Texture" });					
					stack.push_back(keyword);
					keyword.clear();
					continue;
				}
				else if (keyword == "Shader")
				{
					AddDebugLog({ "Keyword: ", "Shader" });
					stack.push_back(keyword);
					keyword.clear();
					continue;
				}
				else
				{
					AddDebugLog({ "Error: ", "Invalid keyword after '#'", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
					stack.push_back(keyword);
					keyword.clear();
					return;
				}
			}
			else if (aStr[pointer] == '[')//��Ϣ��ȡ��Ŀǰ�����ɫ���� �з����Żᵼ����Ϣ��ȡ��ȫ
			{
				pointer++;
				while (aStr[pointer] != ']')//ѭ��ʶ����ĸ
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}
				if (stack.back() == "name")
				{
					AddDebugLog({ "Message: ", keyword });					
					keyword.clear();
					continue;

				}
				else if (stack.back() == "path")
				{
					AddDebugLog({ "Message: ", keyword });	
					keyword.clear();
					continue;
				}
				else if (stack.back() == "vertex")
				{
					AddDebugLog({ "Message: ", keyword });
					keyword.clear();
					continue;
				}
				else if (stack.back() == "fragment")
				{
					AddDebugLog({ "Message: ", keyword });
					keyword.clear();
					continue;
				}
				else if (stack.back() == "geometry")
				{
					AddDebugLog({ "Message: ", keyword });
					keyword.clear();
					continue;
				}
			}
			else if (aStr[pointer] == ']')
			{
				pointer++;
				continue;
			}

		}
		else if (IsLetter(aStr[pointer]))//���3 letter
		{
			//ѭ��ʶ��
			while (IsLetter(aStr[pointer]))//��ĸ��ͷ�ĺ��һ����[ ���һ�����ַ�������ɫ����
			{
				keyword.push_back(aStr[pointer]);
				pointer++;
			}
			if (keyword == "name")
			{
				AddDebugLog({ "Keyword: ", "name" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "path")
			{
				AddDebugLog({ "Keyword: ", "path" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "vertex")
			{
				AddDebugLog({ "Keyword: ", "vertex" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "fragment")
			{
				AddDebugLog({ "Keyword: ", "fragment" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "geometry")
			{
				AddDebugLog({ "Keyword: ", "geometry" });
				stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else
			{
				AddDebugLog({ "Error: ", "Invalid keyword.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
				keyword.clear();
				return;
			}
		}
		else//���4 �� ��
		{
			AddDebugLog({ "Error: ", "Invalid input.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
			return;
		}
		//���5 ����

		//����
	}
}

int CeMaterialLexer::LexicalAnalysis(const string & aStr, vector<ShaderPrototype>* aShader, 
	MaterialPrototype* aMaterial)
{
	int pointer = 0;//�ı�ɨ��ָ��
	int line = 1;//�м�����
	string keyword;//�ؼ���
	vector<string>  keyword_stack;//���﷨ջ
	vector<string> tag_stack;
	ShaderPrototype pro;
	mDebugLog.clear();//�����֮ǰ����־
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
		else if (IsSaperater(aStr[pointer]))//���2 �Ƿ������жϹؼ���
		{
			
			if (aStr[pointer] == '[')//��Ϣ��ȡ(��������·������ɫ��)Ŀǰ�����ɫ���� �з����Żᵼ����Ϣ��ȡ��ȫ
			{				
				pointer++;
				while (aStr[pointer] != ']')
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}			 

				if (keyword_stack.back() == "name")
				{
					AddDebugLog({ "Message: ", keyword });
					pro.texture_name .push_back( keyword);
					keyword.clear();
					continue;
				}
				else if (keyword_stack.back() == "path")
				{
					AddDebugLog({ "Message: ", keyword });
					pro.texture_path.push_back(keyword);
					keyword.clear();
					continue;
				}
				else if (keyword_stack.back() == "vertex")
				{					
					pro.vertex = CeLoader::LoadText(keyword);
					keyword.clear();
					continue;
				}
				else if (keyword_stack.back() == "fragment")
				{
					pro.fragment = CeLoader::LoadText(keyword);
					keyword.clear();
					continue;
				}
				else if (keyword_stack.back() == "geometry")
				{
					pro.geometry = CeLoader::LoadText(keyword);
					keyword.clear();
					continue;
				}
				else if(keyword_stack.back() == "color")
				{
					vector<float> temp_color = TransformStrToFloat(keyword);
					for (int i =0;i < temp_color.size() && i < 3;i++)
					{
						(*aMaterial).color[i] = temp_color[i];
					}								
					keyword.clear();
					continue;
				}
			}
			//�����Ƕ�ȡ�������ǵ�������������رձ�ǵ�ʱ������һЩ���飬�����־һ����ɫ��ԭ�ζ�ȡ���
			else if (aStr[pointer] == '<')//�ж��Ƿ���<>��ǵĹؼ��֣���������������
			{
				pointer++;
				while (aStr[pointer] != '>')
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}
				if (IsOpenTag(keyword))//�ж��Ƿ��Ƕ��俪ʼ��ǩ���������ô�ʹ���tagջ
				{
					AddDebugLog({ "Log: Enter scope. ", keyword });
					tag_stack.push_back(keyword);
					keyword.clear();
					continue;
				}
				else if (HasOpenTag(keyword, tag_stack))//��mtagջ���Ƿ��ж�Ӧ�Ŀ�ʼ��ǩ
				{
					if (keyword == "/shader")//��shader���䣬��Ҫ�����shderԭ�δ浽�������
					{
						AddDebugLog({ "Log: End of scope. ", keyword });
						//���ü����ɫֵ�ǲ���3��
						
						//����ȡ����Ϣ����������						
						(*aShader).push_back(pro);
						tag_stack.pop_back();
						keyword.clear();
						pro.clear();
						continue;
					}
					else if (keyword == "/texture")//��texture���䣬ʲôҲ���ø�
					{
						AddDebugLog({ "Log: End of scope. ", keyword });

						tag_stack.pop_back();
						keyword.clear();
						continue;
					}
					else if (keyword == "/code")//��code���䣬ʲôҲ���ø�
					{
						AddDebugLog({ "Log: End of scope. ", keyword });

						tag_stack.pop_back();
						keyword.clear();
						continue;
					}
					else if (keyword == "/material")
					{
						AddDebugLog({ "Log: End of scope. ", keyword });
						tag_stack.pop_back();
						keyword.clear();
						continue;
					}
				}
				else
				{
					AddDebugLog({ "Error: Scope exeption. ", "@line ", to_string(line), " ", keyword });
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
			else if(aStr[pointer] == '#')
			{
				AddDebugLog({"Error: Now invalid token. ", "@line ", to_string(line), " ", "#" });
				return 0;
			}
		}
		else if (IsLetter(aStr[pointer]))//���3 letter �ж��ؼ���
		{
			//ѭ��ʶ��
			while (IsLetter(aStr[pointer]))//��ĸ��ͷ�ĺ��һ����[ ���һ�����ַ�������ɫ����
			{
				keyword.push_back(aStr[pointer]);
				pointer++;
			}
			if (keyword == "name")
			{
				AddDebugLog({ "Keyword: ", "name" });
				keyword_stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "path")
			{
				AddDebugLog({ "Keyword: ", "path" });
				keyword_stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "vertex")
			{
				AddDebugLog({ "Keyword: ", "vertex" });
				keyword_stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "fragment")
			{
				AddDebugLog({ "Keyword: ", "fragment" });
				keyword_stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if (keyword == "geometry")
			{
				AddDebugLog({ "Keyword: ", "geometry" });
				keyword_stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else if(keyword == "color")
			{
				AddDebugLog({ "Keyword: ", "color" });
				keyword_stack.push_back(keyword);
				keyword.clear();
				continue;
			}
			else
			{
				AddDebugLog({ "Error: ", "Invalid keyword.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
				keyword.clear();
				return 0;
			}
		}
		else//���4 ����
		{
			AddDebugLog({ "Error: ", "Invalid input.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
			return 0;
		}
		//���5 ����

		//����
	}	

	return 1;
}

bool CeMaterialLexer::IsNumber(char aChar)
{
	if (aChar >= '-' && aChar <= '9') return true;
	return false;
}

bool CeMaterialLexer::IsSkip(const char aChar)
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

bool CeMaterialLexer::IsKeyWord(const string & aStr)
{
	for (auto val : mKeyWord)
	{
		if (val == aStr)
		{
			return true;
		}
	}
	return false;
}

bool CeMaterialLexer::IsSaperater(const char aChar)
{
	for (auto val : mSaperater)
	{
		if (val == aChar)
		{
			return true;
		}
	}
	return false;
}

bool CeMaterialLexer::IsLetter(char aChar)
{
	if (aChar >= 'A' && aChar <= 'Z') return true;
	if (aChar >= 'a' && aChar <= 'z') return true;
	return false;
}

void CeMaterialLexer::AddDebugLog(initializer_list<string> aList)
{
	for (auto val : aList)
	{
		mDebugLog += val;
	}
	mDebugLog.push_back('\n');

}


bool CeMaterialLexer::IsOpenTag(const string & aTag)
{
	if (!aTag.empty())
	{
		if (IsLetter(aTag[0]))
			return true;
	}
	return false;
}

bool CeMaterialLexer::HasOpenTag(const string & aTag, const vector<string>& aTags)
{
	if (aTag.empty())return false;
	string temp_str(aTag, 1);
	if (!aTags.empty())
	{
		if (aTags.back() == temp_str)
			return true;
	}
	return false;
}

vector<float> CeMaterialLexer::TransformStrToFloat(const string & aStr)
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
	return nums;
}
