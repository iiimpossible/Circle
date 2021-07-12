#pragma once
#include <vector>
#include <Celib_x64/src/CeCommon/Common.h>
#include "Mesh.h"
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
using std::vector;
using CircleEngine::Vec3;
using CircleEngine::Vec2;

/**
* @brief һ�����ݴ洢�ṹ���洢ģ�͵ĸ�ʽ���Ķ�����Ϣ����ΪMeshRender�Ķ�����Ϣ��Դ��
* @detail �����ֻ�洢�����VAO��VBO�Լ�EBO������ԭʼ���������ڼ�����ģ�ͺ�ʹ���ã��첽���أ�
* @default Ĭ�ϵ�����Cube�������ô�ɫ3d��ɫ����InlineShader��
* @TODO: ��̬������ĵ������ܻ��õ�VBO
*/
class StaticMesh 
{
public:

	StaticMesh();
	~StaticMesh();

public:
	//��������

	inline void SetVAO(unsigned aVAO) { mVAOs.push_back(aVAO); }

	inline void SetVBO(unsigned aVBO) { mVBOs.push_back(aVBO); }

	inline void SetEBO(unsigned aEBO) { mEBOs.push_back(aEBO); }

	inline void SetIndicesNum(unsigned aNum) { mIndicesNum.push_back(aNum); };
public:
	//������
	
	inline void SetMultiVAO(const vector<unsigned>& aVAOs) { mVAOs = aVAOs; }

	inline void SetMultiVBO(const vector<unsigned>& aVBOs) { mVBOs = aVBOs; }

	inline void SetMultiEBO(const vector<unsigned>& aEBOs) { mEBOs = aEBOs; }

	inline void SetMultiIndicesNum(const vector<unsigned>& aIndicesNum) { mIndicesNum = aIndicesNum; };

	inline vector<unsigned> GetMultiVAO() const { return mVAOs; }

	inline vector<unsigned> GetMultiVBO() const { return mVBOs; }

	inline vector<unsigned> GetMultiEBO() const { return mEBOs; }

	inline vector<unsigned> GetIndicesNum()const { return mIndicesNum; }

public:


private:	

	vector<unsigned> mEBOs;
	vector<unsigned> mVAOs;
	vector<unsigned> mVBOs;
	vector<unsigned> mIndicesNum;	

	/** �����֮ǰ���Զ�����ģ�͵���ʱ���Ʒ���*/
	/*void TestDraw();*/
};

 