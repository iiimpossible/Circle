#include "RenderCommand.h"
#include "../Object/GameObject.h"
#include "../Object/Camera.h"
#include "../Component/SpriteRenderer.h"



RenderCommand::RenderCommand(GameObject* aObject)
{	
	if (!aObject) return;
	const RenderInfo info = aObject->GetRenderInfo();

	if (info.bIgnore == false)
	{
		mVAOIDs = info.vao_hds;
		mEBOs = info.ebo_hds;
		mIndicesNum = info.indices_nums;
		//mShaderHDs = aInfo.shader_hds;
		mshaderElements = info.shader_elements;
		mPos = aObject->GetObjectPosition();
		mRotation = aObject->GetObjectRotation();
		mSize = aObject->GetObjectSize();
	}
	else
	{
		bIgnore = true;
	}
	 
}



RenderCommand2D::RenderCommand2D()
{
}

RenderCommand2D::~RenderCommand2D()
{
}
