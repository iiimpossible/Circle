#pragma once
#include <vector>
#include <string>
#include <Celib_x64/src/CeCommon/Common.h>
using std::string;
using std::vector;
using CircleEngine::Vec3;


/**
* @brief ������������������ѧ���Ե�һ�����ݵļ���
* @detail ���������Ѿ������Դ������������ɫ�����ڳ�ʼ����ʱ�򣬽�����������ɫ����
* �����ļ�ʵ������һ���򵥵Ľű��ļ�����Ҫдһ���ض��Ĵʷ��������Խ����е���Ϣ��ȡ����
* TODO��֧�ֶ�����,֧��������Ⱦ
* �����ﴴ��һ�����ز���֡���壬��һ���н�֡���壬
* ����Ⱦ�����ز���֡�������ɫ���ݸ��Ƶ��н�֡���壬
* ���н�֡������Ϊ����

* �Ժ�ʵ�֣�һ�������б�������������б��������������ɫ����������֧�ּ򵥲��ʱ༭
*/
class Material
{
public:
	Material(bool aIsLight = false);
	Material(const vector<ShaderElement>& aShader);
	//Material(const Material& aMaterial);
	//void operator=(const Material& aMaterial);
	~Material();
public:
	//�������

	inline const Vec3& GetEmissiveColor() { return mLightColor; }//������c

	void SetEmissiveColor(const Vec3& aColor);

	void SetEmissiveColor(float aR, float aG, float aB);


public:
	//��ɫ����������

	/** ���½ӿڣ������ɫ���������*/
	inline vector<unsigned> GetShaderHds() const { return mShaderHds; }

	/** ��Ҫɾ��*/
	inline vector<unsigned> GetTextureHds() const { return mTexture2DHds; }

	inline vector<ShaderElement> GetShaderElements() const { return mShaderElemets; }

	inline string GetPath() const { return mPath; }

public:

	inline void SetShaderHDs(const vector<unsigned>& aShaderHDs) { mShaderHds = aShaderHDs; }

	inline void SetShaderElements(const vector<ShaderElement>& aShaderElements) { mShaderElemets = aShaderElements; }

	inline void SetPath(const string& aPath) { mPath = aPath; }

	inline void Clear() { mShaderElemets.clear(); mTexture2DHds.clear(); mShaderHds.clear(); }
public:
	//�Ժ�Ŀɱ༭����

	/** ���������洢��Ϊ��ɾ�����õ�������Ϊ��Щ���������ٴ洢����Դ����������*/
	inline void AddTexture(unsigned aTextureHandle) { mTexture2DHds.push_back(aTextureHandle); }

	void AddShaderElement(unsigned aShaderHd, const vector<unsigned>& aTexHds, const vector<string>& aTexNames);
	 
	inline void AddTexture(const vector<unsigned> aTexHDs) { mTexture2DHds = aTexHDs; }

	inline void AddShader(unsigned aShaderHD) { mShaderHds.push_back(aShaderHD); }
	

	inline void SetShaderConfig(const string& aValue) { mShaderConfig.push_back(aValue); }

private:
	//��������  �Ƿ񷺹⣬����ǿ�� ��Ӱ ���� ���� 
	Vec3 mLightColor;
private:	
	//�������ɫ��

	vector<string> mShaderConfig;

	//�洢���е������������ն�ȡ˳��洢������ʹ��һ��map��map�������������
	vector<unsigned> mTexture2DHds;

	vector<unsigned> mShaderHds;	

	vector<ShaderElement> mShaderElemets;

	string mPath;
	
	bool bIsLight;
};

 