#pragma once
 /**
 * @brief 一个数据存储类，携带了渲染最简单图形所需求的最少的资源
 * 一个着色器句柄，一个顶点数组句柄，以及一个纹理句柄
 * 使用顶点直接渲染（不使用索引）
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
	unsigned mTextureID;//可能是立方体纹理句柄
	int mVerticesNum;
};