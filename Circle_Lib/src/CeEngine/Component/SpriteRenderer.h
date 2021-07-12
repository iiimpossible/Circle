
#include <tuple>
#include <Celib_x64/src/CeCommon/Common.h>
#include "Component.h"
#include "../Render/Material.h"
#include "../Render/Sprite.h"
using std::tuple;
using namespace CircleEngine;

/**
*@brief 2D渲染类，包含2D渲染的所有信息
*@details 包括贴图，着色器等

当精灵渲染组件的shader 和sprite为空的时候，不应该继续创建渲染命令

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
	//着色器和纹理由资源管理器管理，这里只是引用	
	Sprite mSprite;
	unsigned mVAO;
	unsigned mVBO;
	unsigned mEBO;
	
private:
	void SetMesh();
	inline void Init(GameObject* aObject);
};

 