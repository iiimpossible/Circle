#include "Material.h"
#include "../Shader.h"
#include "../Texture2D.h"
#include "../../BOCommon/ResourceManager.h"
Material::Material()
{
	this->m_texture = &ResourceManager::GetTexture("Pre_Box");
	this->m_shader = &ResourceManager::GetShader("Pre_Test");
}
Material ::

Material::~Material()
{
}

unsigned int Material::GetTextureID()
{
	return this->m_texture->GetTexture2DID();
}

unsigned int Material::GetShaderProgramID()
{
	return this->m_shader->GetShaderProgramID();
}

void Material::SetTexture()
{
}

void Material::SetShader()
{
}
