#include <Celib_x64/src/CeEngine/Object/GameObject.h>

class CameraComponent;
class Scene;
class CameraActor:public GameObject
{
public:
	CameraActor (const string& aName = "CameraActor");
	~CameraActor ();

public:

	virtual void Start()override;
	virtual void Update() override;


private:

	//����ӳ�����ӳ��
	void OnRightButtonPressed(float aTag);

	void OnRightButtonRelease(float aTag);

	void MoveForward(float aValue);

	void MoveRight(float aValue);
	//�����ǣ�X��
	void LookUp(float aPitch);
	//ƫ����Y��
	void LookRight(float aYaw);

	void ProcessMouseScroll(float yoffset);

private:

	Scene* mScene;

	CameraComponent* mCameraComp;

	bool isRightButtonPressed;
};

REGISTER(CameraActor);
 