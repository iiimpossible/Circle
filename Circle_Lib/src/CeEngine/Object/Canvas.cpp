#include "Canvas.h"
#include "../Component/SpriteRenderer.h"
#include "../Manager/ResourceManager.h"
 
Canvas::Canvas(string aName) :
	GameObject(aName)
{
	RendererComp = new SpriteRenderer(this);
	this->RendererComp->SetShader(ResourceManager::GetShader("Pre_RealTimeAnim"));
	RendererComp->SetSprite(ResourceManager::GetTexture("Pre_White"));
	this->SetObjectSize(Vec3(ViewportWidth, ViewportHeight, 1.0f));
	this->SetObejctPosition(Vec3(0.0f, 600.0f, 0.0f));
}

Canvas::~Canvas()
{
}

RenderInfo Canvas::GetRenderInfo()
{
	//DebugData("");
	//return RenderInfo{ RendererComp->GetShaderHandle(),RendererComp->GetVertexArrayHandle(),
		//RendererComp->GetSpriteHandle() };
	return {};
}
