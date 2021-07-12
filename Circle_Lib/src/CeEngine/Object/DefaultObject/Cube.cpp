#include "Cube.h"
#include "../../Component/MeshRenderer.h"
#include "../../../CCCommon/Tool/DefaultData.h"
#include "../../Manager/ResourceManager.h"
#include "../../Render/Texture2D.h"
Cube::Cube(string aName):
	GameObject(aName)
{
	this->MeshComp = new MeshRenderer(this);
	
	
	MeshComp->SetTexture2D(ResourceManager::GetTexture("Pre_Box"));
	MeshComp->mMaterial.Init();
	/*this->SetObjectSize(Vec3(0.3f));*/
	//this->SetObejctPosition(Vec3(10.0f, 10.0f, 10.0f));

	//this->mTestVAO = CubeTestVAO();
}

Cube::~Cube()
{
}

RenderInfo Cube::GetRenderInfo() const
{
	return RenderInfo{ this->MeshComp->GetShaderHandle(),MeshComp->GetVertexArrayHandle(),
		this->MeshComp->GetTextureHandle()};
}
