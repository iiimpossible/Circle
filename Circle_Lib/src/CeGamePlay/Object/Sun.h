#include <CeEngine/Object/GameObject.h>

class LightComponent;
class Sun:public GameObject
{
public:
	Sun(const string& aName = "Sun");
	~Sun();

public:

	void Start() override;

	void Update() override;


private:
	LightComponent* mLightComp;
};
 
REGISTER(Sun);