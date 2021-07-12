#include <CeEngine/Object/GameObject.h>


class PointLight:public GameObject
{
	PointLight(const string& aName = "PointLight");

	~PointLight();


public:

	virtual void Start() override;

	virtual void Update() override;


private:

	class LightComponent* mLightComp;

};