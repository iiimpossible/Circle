#include <string>
#include <vector>
#include <initializer_list>
#include <CeCommon/Common.h>
using std::initializer_list;
using std::vector;
using std::string;
//一个简单的指令集
enum EInstruction
{
	Set_Vec3 = 0x01,
	Set_Vec2 = 0x02,
	Set_Mat4 = 0x03,
	Set_Float = 0x04,
	Set_Bool = 0x05,
	Set_Int = 0x06,
	Set_String = 0x07

};


/**
 * @breif 一个简单的虚拟机，分析并执行一些简单的指令
 * @detail 目前支持的指令，3D向量
 */
class CeVM
{
public:
	/** @breif 单条指令解释*/
	static string Interpreter(EInstruction aIns,const string& aCommand);


public:
	//工具函数

	static Vec3 StrToVec3(const string& aCommand);

	static vector<Vec3> StrToTransform(const string& aCommand);

	static Vec2 StrToVec2(const string& aCommand);

	static Mat4 StrToMat4(const string& aCommand);

private:
	static bool IsNumber(char aChar);

	static bool IsLetter(char aChar);

	static bool IsSeparater(char aChar);

	static bool IsSkip(char aChar);

	static bool IsKeyword(const string& aWord);

	static void AddDebugLog(initializer_list<string> aList);

	static vector<float> StrToFloat(const string& aStr, const unsigned aDemension = 3);

	static string RemoveBracket(const string& aStr);
private:

	static string CeVMLexer(const string& aCode);

	//关键字
	static vector<string> mKeyword;
	
	//界符
	static vector<char> mSeparater;
	//跳过符（直接跳过/过滤的符号）空格、换行、回车、tab
	static vector<char> mSkip;
	//数字
	static vector<char> mNumber;
	//调试日志
	static string mDebugLog;

};