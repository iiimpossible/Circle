
#include <tuple>
#include <Celib_x64/src/CeCommon/Common.h>
#include "Component.h"
#include "../Render/Material.h"
#include "../Render/Sprite.h"
using std::tuple;
using namespace CircleEngine;

/**
*@brief 2D��Ⱦ�࣬����2D��Ⱦ��������Ϣ
*@details ������ͼ����ɫ����

��������Ⱦ�����shader ��spriteΪ�յ�ʱ�򣬲�Ӧ�ü���������Ⱦ����

*/

class Shader;
class Sprite;
class Texture2D;
class Component;
class GameObject;
 

class SpriteRenderer:public Component
{
public:	
	SpriteRenderer(GameObject* aObject = nullptr);
	~SpriteRenderer();
public:
	virtual unsigned GetVertexArrayHandle() const;
	virtual unsigned GetShaderHandle() const;
	virtual unsigned GetSpriteHandle() const;

	void SetShader(const Shader& aShader);//a = argument
	void SetSprite(const Texture2D& aSprite);

	void SetVAO(unsigned aVAO);
	void SetVBO(unsigned aVBO);
	void SetEBO(unsigned aEBO);
 
	Sprite& GetSprite();

	virtual void SetCommand(const string& aCmd)override;
public:

	virtual void Start() override;
	virtual void Update() override;
protected:
	//��ɫ������������Դ��������������ֻ������	
	Sprite mSprite;
	unsigned mVAO;
	unsigned mVBO;
	unsigned mEBO;
	
private:
	void SetMesh();
	inline void Init(GameObject* aObject);
};

 