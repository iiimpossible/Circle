#pragma once
#include "SceneObject.h"
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
#include <Celib_x64/src/CeEngine/Object/ObjectFactory.h>

class MeshRenderer;
 /**
 * @breif ��Ϸ�����࣬������������Ϸ������Ļ���
 * @detail ������3D��Ϸ������ࡣ����һ��MeshRender������������������Ͳ��ʣ���
 * ��Ⱦ����ı�Ҫ��Ϣ
 * @TODO:��������������գ�Ӧ������SceneObject��������һ��������ô洢����ִ��Object������������ʱ��
 * �����е��������
 */
class GameObject: public SceneObject
{
public: 

	GameObject(const string& aName = "GameObject");
	GameObject(GameObject& aGObject) = delete;
	GameObject& operator=(GameObject& aGObject) = delete; 
	virtual ~GameObject();
public:

	void SetMaterial(const string& aMaterailName);	

	//һ�����麯�����ڻ����Ⱦ����������Ϣ��������ɫ����������������
	RenderInfo GetRenderInfo() const;

	
public:

	virtual void Start() override;
	virtual void Update() override;
protected:
	 
	//Ҫ���������Ⱦ��Ϣ�ṩ�����ջ��������ټ���2D��Ϸ�����ˣ�2D��Ϸ���������һ������Ĳ���Z���ƶ������
	//��2D��Ϸ���壬�����������ܺܵ͡�
	MeshRenderer* mMeshRender; 
};




