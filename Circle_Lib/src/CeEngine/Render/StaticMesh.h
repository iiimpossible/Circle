#pragma once
#include <vector>
#include <Celib_x64/src/CeCommon/Common.h>
#include "Mesh.h"
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
using std::vector;
using CircleEngine::Vec3;
using CircleEngine::Vec2;

/**
* @brief 一个数据存储结构，存储模型的格式化的顶点信息，作为MeshRender的顶点信息资源类
* @detail 这个类只存储网格的VAO、VBO以及EBO。对于原始顶点数据在加载完模型后就处理好（异步加载）
* @default 默认的网格（Cube）与内置纯色3d着色器（InlineShader）
* @TODO: 动态对网格的调整可能会用到VBO
*/
class StaticMesh 
{
public:

	StaticMesh();
	~StaticMesh();

public:
	//单个网格

	inline void SetVAO(unsigned aVAO) { mVAOs.push_back(aVAO); }

	inline void SetVBO(unsigned aVBO) { mVBOs.push_back(aVBO); }

	inline void SetEBO(unsigned aEBO) { mEBOs.push_back(aEBO); }

	inline void SetIndicesNum(unsigned aNum) { mIndicesNum.push_back(aNum); };
public:
	//多网格
	
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

	/** 这个是之前测试多网格模型的临时绘制方法*/
	/*void TestDraw();*/
};

 