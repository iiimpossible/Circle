#include "LightManager.h"
#include <CeEngine/Component/LightComponent.h>
#include <CeEngine/Render/Renderer.h>

list<LightComponent*> LightManager::mLightComponents;

LightManager::LightManager()
{

}

LightManager::~LightManager()
{
}

void LightManager::Start()
{
}

void LightManager::Update()
{
	InsertCommand();
}

void LightManager::InsertCommand(const LightCommand & aLCmd)
{
	Renderer::InsertLightCommand(aLCmd);
}

void LightManager::InsertLightComponent(LightComponent * aComponent)
{
	mLightComponents.push_back(aComponent);
}

void LightManager::DeleteLightComponent(LightComponent * aComponent)
{
	for (auto iter = mLightComponents.begin(); iter != mLightComponents.end(); iter++)
	{
		if ((*iter) == aComponent)
		{
			(*iter) == 0;
			iter = mLightComponents.erase(iter);
			break;
		}
	}
}


void LightManager::InsertCommand()
{
	auto iter = mLightComponents.begin();
	while (iter!=mLightComponents.end())
	{
		if (!(*iter))//�����ڳ������ɾ������Ϸ���岢��ָ����Ϊ0�ˣ����ﻹ�ǲ��ܼ�⵽�գ�
		{
			iter = mLightComponents.erase(iter);
		}
		else
		{
			Renderer::InsertLightCommand( (*iter)->GetLightInfo());
			iter++;
		}
		
	}

}
	 
	
