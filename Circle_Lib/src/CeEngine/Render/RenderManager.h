#pragma once

#include <CeCommon/Common.h>
class GameObject;
class RenderCommand;
class RenderCommand2D;
class Camera;
/**
* @brief ִ����Ⱦ����
* @details ����Ⱦ�������뵽��Ⱦ���С�
* �߼���������Ϸ��������ʱ����ȡ��Ϸ�������Ϣ��������Ⱦ���
* ʵ�ʣ�һ�������������
*/
class RenderManager
{
public:
	RenderManager();
	~RenderManager();
public:
	//�������������ʳ�ʼ��

	void InitRenderSource();

public:
	//�����

	/** @breif ����һ��3D��Ⱦ�������ע�ᵽ��Ⱦ������*/
	static void CreateRenderCommand3D(GameObject* aObjet);
	 
	/** @breif ����Ⱦ���������Ⱦ���У�ǿ���*/
	static void InsertCommand(RenderCommand* aRC);	 
 
	/** @breif ����Ⱦ��������2D��Ⱦ����*/
	static void InsertCommand(RenderCommand2D* aRC);

	static void InsertCameraInfo(const CameraInfo& aInfo);
};

