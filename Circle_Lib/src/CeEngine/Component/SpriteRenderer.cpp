#include "SpriteRenderer.h"
#include <Celib_x64/src/CeCommon/OpenGL/CCGLDraw.h>
#include "../Render/Shader.h"
#include "../Render/Texture2D.h"
#include "../Object/GameObject.h"
#include "../Manager/ResourceManager.h"

float QuadVerCoord[12]
{
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,0.0f,

	1.0f,1.0f,
	0.0f,1.0f,
	0.0f,0.0f
};

/* 应该使用不交叉的数据分配，太麻烦*/

float QuadTexCoord[12]
{
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,0.0f,

	1.0f,1.0f,
	0.0f,1.0f,
	0.0f,0.0f  
};

 



SpriteRenderer::SpriteRenderer(GameObject * aObject) :
	Component(aObject)
{
	Init(aObject);	
}
 
	 

SpriteRenderer::~SpriteRenderer()
{
}

unsigned SpriteRenderer::GetVertexArrayHandle() const
{
	return mVAO;
}

unsigned SpriteRenderer::GetShaderHandle() const
{	
	return mSprite.GetShader().GetShaderHandle();
}

unsigned SpriteRenderer::GetSpriteHandle() const
{
	return mSprite.GetTexture2D().GetTexture2DHandle();
}

void SpriteRenderer::SetShader(const Shader & shader)
{
	mSprite.SetShader(shader);
}

void SpriteRenderer::SetSprite(const Texture2D & sprite)
{
	mSprite.SetTexture2D(sprite);
}

void SpriteRenderer::SetVAO(unsigned VAO)
{
	mVAO = VAO;
}

void SpriteRenderer::SetVBO(unsigned VBO)
{
	mVBO = VBO;
}

void SpriteRenderer::SetEBO(unsigned EBO)
{
	mEBO = EBO;
}

Sprite & SpriteRenderer::GetSprite()
{
	return mSprite;
}

void SpriteRenderer::SetCommand(const string & aCmd)
{
	if (aCmd.empty()) return;
	 
	mCurrentCommand = aCmd;
}

void SpriteRenderer::Start()
{
}

void SpriteRenderer::Update()
{
}
 

void SpriteRenderer::SetMesh()
{
	mVAO = CreateVAO();
	//DebugData("@SpriteRenderer::SetMesh >> ", mVAO);
	//CreateVBO(m_VAO, QuadVerCoord, 18, 3, 3);
	mVBO = CreateVBO(mVAO, std::make_tuple(QuadVerCoord, 12, 2, 2), std::make_tuple(QuadTexCoord, 12, 2, 2));
}

void SpriteRenderer::Init(GameObject * aObject)
{
	if (aObject)
	{
		mSprite.SetShader(ResourceManager::GetShader("Pre_Base2D"));
		SetMesh();
	}
}
