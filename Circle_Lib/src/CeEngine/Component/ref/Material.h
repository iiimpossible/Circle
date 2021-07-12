#pragma once
#include "Component.h"
#include <vector>

class Texture2D;
class Shader;
 /**
 * @brief ���ʣ��������Ĺ���������
 * @details ���ʰ�������ɫ������������һЩ���ղ��������������һЩ�������ʡ�
 * TODO::Ӧ�ý����ղ������Ϊһ���ṹ�壬�߹⣬�����䣬������
 */
class Material :
	public Component
{
public:
	///����
	Material();
	~Material();
	unsigned int GetTextureID();
	unsigned int GetShaderProgramID();
	void SetTexture();
	void SetShader();
	///����
	Texture2D* m_texture;
	Shader* m_shader;
	vector<Texture2D> textures;
};
