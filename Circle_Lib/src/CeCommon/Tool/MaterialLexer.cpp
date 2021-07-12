#include "MaterialLexer.h"
#include "CeLoader.h"
using namespace std;

string CeMaterialLexer::mDebugLog;
vector<string> CeMaterialLexer::mKeyWord{"color","ambient", "diffuse", "specular","name","path","vertex","fragment","geometry" };
//#是关键字的标志，$是着色器的标志，[]是路径标志
vector<char> CeMaterialLexer::mSaperater{ '#','[',']','<','>','"' };//TODO:为了与着色器代码中的[]和<>区分，未来vertex[]的[]中要添加"
vector<char> CeMaterialLexer::mSkip{ ' ','\n','\t','\r' };//空格 换行 制表符 回车
vector<string> CeMaterialLexer::mSymbolTable;

CeMaterialLexer::CeMaterialLexer()
{
}

CeMaterialLexer::~CeMaterialLexer()
{
}

void CeMaterialLexer::LexicalAnalysis(const string& aStr, vector<string>* aName, vector<string>* aPath, ShaderCode* aCode)
{
	//方括号计数器	 
	int SqBracketCounter = 0;
	int pointer = 0;//文本扫描指针
	int line = 1;//行计数器
	string keyword;//关键字
	vector<string>  stack;//简单语法栈

	mDebugLog.clear();//先清除之前的日志

	while (pointer < aStr.size())
	{
		if (IsSkip(aStr[pointer]))//起点1 是否跳过符
		{
			if (aStr[pointer] == '\n')
			{
				line++;
			}
			pointer++;
			continue;
		}
		else if (IsSaperater(aStr[pointer]))//起点2 是否界符以判断关键字
		{
			if (aStr[pointer] == '#')//定义#后边一定更关键字
			{
				pointer++;
				while (IsLetter(aStr[pointer]))//循环识别字母
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
			else if (aStr[pointer] == '[')//信息提取，目前如果着色器中 有方括号会导致信息读取不全
			{
				++SqBracketCounter;
				pointer++;
				for (; pointer < aStr.size(); pointer++)//循环识别字母
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
		else if (IsLetter(aStr[pointer]))//起点3 letter 判定关键字
		{
			//循环识别
			while (IsLetter(aStr[pointer]))//字母打头的后边一定跟[ 后边一定跟字符串（着色器）
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
		else//起点4 “ ”
		{
			AddDebugLog({ "Error: ", "Invalid input.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
			return;
		}
		//起点5 数字

		//其他
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
		if (IsSkip(aStr[pointer]))//起点1 是否跳过符
		{
			if (aStr[pointer] == '\n')
			{
				line++;
			}
			pointer++;
			continue;
		}
		else if (IsSaperater(aStr[pointer]))//起点2 是否界符
		{
			if (aStr[pointer] == '#')//定义#后边一定更关键字
			{
				pointer++;
				while (IsLetter(aStr[pointer]))//循环识别字母
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
			else if (aStr[pointer] == '[')//信息提取，目前如果着色器中 有方括号会导致信息读取不全
			{
				pointer++;
				while (aStr[pointer] != ']')//循环识别字母
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
		else if (IsLetter(aStr[pointer]))//起点3 letter
		{
			//循环识别
			while (IsLetter(aStr[pointer]))//字母打头的后边一定跟[ 后边一定跟字符串（着色器）
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
		else//起点4 “ ”
		{
			AddDebugLog({ "Error: ", "Invalid input.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
			return;
		}
		//起点5 数字

		//其他
	}
}

int CeMaterialLexer::LexicalAnalysis(const string & aStr, vector<ShaderPrototype>* aShader, 
	MaterialPrototype* aMaterial)
{
	int pointer = 0;//文本扫描指针
	int line = 1;//行计数器
	string keyword;//关键字
	vector<string>  keyword_stack;//简单语法栈
	vector<string> tag_stack;
	ShaderPrototype pro;
	mDebugLog.clear();//先清除之前的日志
	while (pointer < aStr.size())
	{
		if (IsSkip(aStr[pointer]))//起点1 是否跳过符
		{
			if (aStr[pointer] == '\n')
			{
				line++;
			}
			pointer++;
			continue;
		}
		else if (IsSaperater(aStr[pointer]))//起点2 是否界符以判断关键字
		{
			
			if (aStr[pointer] == '[')//信息提取(纹理名、路径、着色器)目前如果着色器中 有方括号会导致信息读取不全
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
			//段落标记读取，段落标记的意义就是遇到关闭标记的时候能做一些事情，比如标志一个着色器原形读取完毕
			else if (aStr[pointer] == '<')//判断是否是<>标记的关键字，用于描述作用域
			{
				pointer++;
				while (aStr[pointer] != '>')
				{
					keyword.push_back(aStr[pointer]);
					pointer++;
				}
				if (IsOpenTag(keyword))//判断是否是段落开始标签，如果是那么就存入tag栈
				{
					AddDebugLog({ "Log: Enter scope. ", keyword });
					tag_stack.push_back(keyword);
					keyword.clear();
					continue;
				}
				else if (HasOpenTag(keyword, tag_stack))//在mtag栈顶是否有对应的开始标签
				{
					if (keyword == "/shader")//出shader段落，需要将这个shder原形存到输出数组
					{
						AddDebugLog({ "Log: End of scope. ", keyword });
						//不用检测颜色值是不是3个
						
						//将读取的信息存入数组中						
						(*aShader).push_back(pro);
						tag_stack.pop_back();
						keyword.clear();
						pro.clear();
						continue;
					}
					else if (keyword == "/texture")//出texture段落，什么也不用干
					{
						AddDebugLog({ "Log: End of scope. ", keyword });

						tag_stack.pop_back();
						keyword.clear();
						continue;
					}
					else if (keyword == "/code")//出code段落，什么也不用干
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
		else if (IsLetter(aStr[pointer]))//起点3 letter 判定关键字
		{
			//循环识别
			while (IsLetter(aStr[pointer]))//字母打头的后边一定跟[ 后边一定跟字符串（着色器）
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
		else//起点4 错误
		{
			AddDebugLog({ "Error: ", "Invalid input.", "@line ", to_string(line), " @char ", to_string(pointer), " ", keyword });
			return 0;
		}
		//起点5 数字

		//其他
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
		if (IsNumber(aStr[pointer]))//1.数字状态
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
			//TODO:数字输入合法性检测。需要将数字提取出来一个一个字符进行语法分析
			//目前如果输出的数字为0.0且与预期不符,那么是输入错错误
		}
		else if (IsSkip(aStr[pointer]))//2.跳过符状态
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
