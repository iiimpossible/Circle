#pragma once
 /**
 * @brief һ�����ݴ洢�࣬Я������Ⱦ���ͼ������������ٵ���Դ
 * һ����ɫ�������һ���������������Լ�һ��������
 * ʹ�ö���ֱ����Ⱦ����ʹ��������
 */
class Shader
{
public:
	Shader(unsigned aShaderHandle = 0);
	~Shader();
	 
	inline unsigned GetShaderHandle() const { return mShaderID; }

	inline void SetShaderHandle(unsigned aShaderHandle) { mShaderID = aShaderHandle; }

	inline unsigned GetVAO() { return mVAO; }
	inline int GetVertexNum() { return mVerticesNum; }
public:
	unsigned mShaderID;
	unsigned mVAO;
	unsigned mVBO;
	unsigned mTextureID;//������������������
	int mVerticesNum;
};