#include "GameObject.h"
#include <Celib_x64/src/CeEngine/Component/MeshRenderer.h>
#include <Celib_x64/src/CeEngine/Render/Material.h>
#include <Celib_x64/src/CeEngine/Render/StaticMesh.h>
#include <Celib_x64/src/CeEngine/Manager/ResourceManager.h>


GameObject::GameObject(const string& aName) :
	SceneObject(aName)
{
	mMeshRender = AddComponent<MeshRenderer>(new MeshRenderer(this));//通过这个方法可以将组件注册到组件容器中，方便释放内存	 
}


GameObject::~GameObject()
{ 

}

void GameObject::Start()
{
}

void GameObject::Update()
{

}

void GameObject::SetMaterial(const string & aMaterailName)
{
	if (ResourceManager::FindMaterial(aMaterailName))
	{
		mMeshRender->SetMaterial(ResourceManager::GetMaterial(aMaterailName));		
	}
}


RenderInfo GameObject::GetRenderInfo() const
{
	RenderInfo info;

	if (mMeshRender->GetIgnore())
	{
		info.bIgnore = true;
		return info;
	}
	//info.shader_hds = mMeshRender->GetMaterail().GetShaderHds();
	info.vao_hds = mMeshRender->GetStaticMesh().GetMultiVBO();
	info.ebo_hds = mMeshRender->GetStaticMesh().GetMultiEBO();
	info.indices_nums = mMeshRender->GetStaticMesh().GetIndicesNum();
	info.shader_elements = mMeshRender->GetMaterail().GetShaderElements();
	return info;
}
 