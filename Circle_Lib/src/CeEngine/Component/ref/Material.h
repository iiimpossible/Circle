#pragma once
#include "Component.h"
#include <vector>

class Texture2D;
class Shader;
 /**
 * @brief 材质，表达物体的光照特征。
 * @details 材质包含了着色器和纹理，还有一些光照参数，调整物体的一些光照性质。
 * TODO::应该将光照参数打包为一个结构体，高光，漫反射，环境光
 */
class Material :
	public Component
{
public:
	///方法
	Material();
	~Material();
	unsigned int GetTextureID();
	unsigned int GetShaderProgramID();
	void SetTexture();
	void SetShader();
	///属性
	Texture2D* m_texture;
	Shader* m_shader;
	vector<Texture2D> textures;
};
