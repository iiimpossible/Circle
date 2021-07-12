#include "RenderManager.h"
#include "RenderCommand.h"
#include "Renderer.h"
#include "../Object/GameObject.h"


 
RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
	
}

void RenderManager::InitRenderSource()
{
}


 

void RenderManager::CreateRenderCommand3D(GameObject * aObjet)
{
	RenderCommand* r = new RenderCommand(aObjet);

	Renderer::InsertRenderCommand(r);

	//Renderer::InsertCameraInfo({aCamera->GetProjectionMatrix(),aCamera->GetViewMatrix()});
}

void RenderManager::InsertCommand(RenderCommand * aRC)
{
	Renderer::InsertRenderCommand(aRC);
}

void RenderManager::InsertCommand(RenderCommand2D* aRC)
{
	Renderer::InsertRenderCommand(aRC);
}

void RenderManager::InsertCameraInfo(const CameraInfo & aInfo)
{
	Renderer::InsertCameraInfo(aInfo);
}



