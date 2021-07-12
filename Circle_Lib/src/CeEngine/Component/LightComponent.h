#pragma once
#include <string>
#include <Celib_x64/src/CeEngine/Component/Component.h>
#include <CeEngine/Object/GameObject.h>
using std::string;

 
class GameObject;
class MeshRenderer;
/**
 * @breif ����Ϸ�����Ϊһ����Դ
 * @detail �ṩ������ɫ���Ƿ񷺹⡢�ȡ��ڹ��캯���н���Ϸ����ע�ᵽrenderer�У�����
 * ��Ⱦ����ȡ��Դ��ɫ��λ�ú�����һЩ����
 */
class LightComponent:public Component
{
public:

	LightComponent(GameObject* aObject, const string& aName = "LightComponent");
	~LightComponent();

public:

	/** @breif �ṩ��Դ����Ĺ�����Ϣ�������չ�����ÿ֡����*/
	const LightCommand& GetLightInfo();

	/** @breif ���ù�Դ�������ɫ��������ɫ*/
	void SetEmissiveColor(const Vec3& aColor);


	virtual void SetCommand(const string& aCmd)override;

public:

	virtual void Start()override;

	virtual void Update() override;

private:

	Mat4 GetLightSpaceMat();


private:

	/** @breif ��ʱ�ľ�̬�Ĺ�������*/
	LightCommand mLightCmd;//

	/** @breif ���ӵ���ߵ�������Ⱦ���*/
	MeshRenderer* mOwnerMeshRenderer;
};

 