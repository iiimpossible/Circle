#include "ScriptManager.h"


vector<TypeRefCallback> ScriptManager::mTypeRefs;
vector<string> ScriptManager::mTypeName;
ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::Start()
{
}

void ScriptManager::Update()
{
}

void ScriptManager::RegistClass(TypeRefCallback aCallback)
{
	mTypeName.push_back(aCallback());
}
