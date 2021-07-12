#pragma once
#include <list>
#include <Celib_x64/src/CeCommon/Common.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
using std::list;
 
 
class LightComponent;


/**
 * @breif 光照管理组件，包括与渲染器通信并传递光照命令
 * @note 光照管理器的Update需要在渲染之前场景处理之后执行
 */
class LightManager
{
public:

	LightManager();
	~LightManager();
public:

	void Start();

	void Update();
public:

	/** @breif 可能的外部光照命令插入*/
	static void InsertCommand(const LightCommand& aCommand);

	/** @breif 注册光照组件*/
	static void InsertLightComponent(LightComponent* aComponent);

	static void DeleteLightComponent(LightComponent* aComponent);
private:

	/** @breif 每帧执行该函数，遍历所有的光照组件，并插入选染信息到渲染器*/
	void InsertCommand();
private:

	//因为可能经常需要删除元素，所以使用list
	static list<LightComponent*> mLightComponents;
};