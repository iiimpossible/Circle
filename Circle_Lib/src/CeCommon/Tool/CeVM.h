#include <string>
#include <vector>
#include <initializer_list>
#include <CeCommon/Common.h>
using std::initializer_list;
using std::vector;
using std::string;
//һ���򵥵�ָ�
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
 * @breif һ���򵥵��������������ִ��һЩ�򵥵�ָ��
 * @detail Ŀǰ֧�ֵ�ָ�3D����
 */
class CeVM
{
public:
	/** @breif ����ָ�����*/
	static string Interpreter(EInstruction aIns,const string& aCommand);


public:
	//���ߺ���

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

	//�ؼ���
	static vector<string> mKeyword;
	
	//���
	static vector<char> mSeparater;
	//��������ֱ������/���˵ķ��ţ��ո񡢻��С��س���tab
	static vector<char> mSkip;
	//����
	static vector<char> mNumber;
	//������־
	static string mDebugLog;

};