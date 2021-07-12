#pragma once

#include <CeCommon/Common.h>
class GameObject;
class RenderCommand;
class RenderCommand2D;
class Camera;
/**
* @brief 执行渲染调度
* @details 将渲染命令输入到渲染器中。
* 逻辑：遍历游戏对象容器时，读取游戏对象的信息，生成渲染命令。
* 实质：一个命令产生工厂
*/
class RenderManager
{
public:
	RenderManager();
	~RenderManager();
public:
	//引擎基本网格材质初始化

	void InitRenderSource();

public:
	//命令处理

	/** @breif 创建一个3D渲染命令并将其注册到渲染队列中*/
	static void CreateRenderCommand3D(GameObject* aObjet);
	 
	/** @breif 将渲染命令插入渲染队列，强耦合*/
	static void InsertCommand(RenderCommand* aRC);	 
 
	/** @breif 向渲染其器插入2D渲染命令*/
	static void InsertCommand(RenderCommand2D* aRC);

	static void InsertCameraInfo(const CameraInfo& aInfo);
};

