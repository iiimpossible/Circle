#include <CeCommon/Tool/CeUtility.h>
#include "MeshRenderer.h"
#include "../Object/GameObject.h"
#include "../Manager/ResourceManager.h"

MeshRenderer::MeshRenderer(GameObject * aObejct):
	Component(aObejct)
{
	mStaticMesh = ResourceManager::GetStaticMesh("Pre_Cube");
	mMaterial = ResourceManager::GetMaterial("Pre_StandardMat");
	bIgnore = false;
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMaterial(const string & aPath, const string& aName)
{
	ResourceManager::LoadMaterial(aPath, aName);
	Material mat = ResourceManager::GetMaterial(aName);
	SetMaterial(mat);
	mCurrentCommand = mat.GetPath();
}

void MeshRenderer::SetMaterialInstance(Material & aMat)
{

}

 

void MeshRenderer::SetCommand(const string & aCmd)
{
	if (aCmd.empty()) return;
	SetMaterial(aCmd, CeUtility::GetFileNameFromPath(aCmd));
	mCurrentCommand = aCmd;
}

void MeshRenderer::Start()
{
}

void MeshRenderer::Update()
{
}
