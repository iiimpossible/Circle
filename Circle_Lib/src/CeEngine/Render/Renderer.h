#pragma once
#include <iostream>
#include <list>
using std::list;

#include <Celib_x64/src/CeCommon/Common.h>
using namespace CircleEngine;
class RenderCommand;
class RenderCommand3D;
class RenderCommand2D;
struct Texture;
class Texture2D;


enum ERenderEffect
{
	ERE_Bloom = 0,//泛光
	ERE_MultiSample_4,//多重采样抗锯齿
	ERE_MultiSample_8,
	ERE_MultiSample_16,
	ERE_Reflect,//反射
	ERE_Shadow,//阴影
	ERE_Refraction,//折射
	ERE_Gamma//γ校正
};


 /**
 * @brief 渲染器，引擎的渲染都是这个类处理的。
 * 渲染逻辑为：传入顶点数据包，配置着色器变量。
 * 输入数据：，着色器id，transform组件，material组件，mesh组件。
 * 渲染系统每一次绘制调用都是一个渲染命令。渲染命令有多种有批绘制，有二维图片绘制，等等。
 */
class Renderer
{
public:
	Renderer();
	~Renderer();
	Renderer(Renderer& renderer) = delete;
	Renderer& operator=(Renderer& renderer) = delete;
public:

	/**
	 * @brief 渲染方法。
	 * 支持渲染2D模型，3D单纹理模型
	 * 支持后处理效果
	 */
	void Update();

	/** @brief 2D渲染方*/
	void Render2D();
 
	/** @breif 这个目前是用来渲染与有向距离场有关的着色器，未来可能用于渲染UI或者另外使用一个方法来渲染UI*/
	void RenderCanvas();

	void InitRender();

	void Start();
	 
	/** @breif 这个是对位掩码的尝试，来开启关闭渲染效果*/
	void SetRenderEffect(ERenderEffect aEffect);
public:
	//渲染命令
	 
	/** @breif 插入一个命令到命令列表中,这个静态函数不能访问类的成员吗？静态函数只能访问静态变量，不能访问类成员变量*/	 
	static void InsertRenderCommand(RenderCommand* aRCmd);

	static void InsertRenderCommand(RenderCommand3D* aRCmd);

	static void InsertRenderCommand(RenderCommand2D* aRCmd);
	 
	/** @breif 这个可能是未来支持多相机渲染的关键，这个信息是为了避免每个游戏物体都要传送相机信息*/	
	static void InsertCameraInfo(const CameraInfo& aInfo);
	
public:
	//光照命令

	static void InsertLightCommand(const LightCommand& aLCmd);
private:

	/**
	 * @brief 配置着色器参数
	 * @details 模型矩阵，视口矩阵，投影矩阵
	 */
	 void SetShader(const unsigned int aShaderHandle, const RenderCommand* aRC);
	/**
	 * @brief 2D着色器配置
	 * @details
	 模型矩阵 进行物体移动
	 视口矩阵 摄像机移动
	 投影矩阵 图像映射
	 */
	 void SetShader2D(const RenderCommand2D* aRC);
	 void SetTexttue2D(const RenderCommand2D* aRC);

	/** @breif 给每个着色器的光照信息赋值*/
	 void SetShaderLight(unsigned aShaderHd);


	 void DrawFrameBuffer();

	 void DrawSkybox();

	 void DrawScene();

	 void DrawShadowMaps();


	 void StartBloom();

	 void EndBloom();

private:
	///A list store gameobject pointer.
	static list<RenderCommand*> mRenderCmds;

	//如果要实现缓存友好的话，那么就应该在外部使用一个类，作为数据中转站，将渲染命令用一个数组存储起来
	static vector<RenderCommand3D*> mRC3DContainer;

	//目前不需要排序，所以使用数组
	static vector<RenderCommand2D*> mRC2DContainer;

	static vector<LightCommand> mLightCmds;

	static CameraInfo mCameraInfo;
private:
	
	//是否启用后处理
	bool bPostProcess;
	//是否启用深度测试
	bool bEnableDepthTest;
	//是否启用模板测试
	bool bEnableStencilTest;
	//是否3D场景
	bool bIs3DScene;
	//是否启用泛光
	bool bEnableBloom;
	//是否启用抗锯齿
	bool bEnableMultiSample;	

private:
	//多重采样相关帧缓冲
	FBOID mMSFBO;	
	Texture2DID mMSBuffer[2];
	Texture2DID mMSDepthbuffer;

	//中介帧缓冲
	FBOID mIntermediateFBO;
	Texture2DID mIntermediateBuffer;

private:
	//泛光效果相关帧缓冲

	ShaderID mBloomShader;//泛光着色器 也可以测试是否成功处理好了帧缓冲
	ShaderID mBlurShader;//高斯模糊着色器
	ShaderID mBloomFinalShader;//泛光最终着色器	
	FBOID mBloomFBO;//泛光帧缓冲		
	RBOID mBloomDepthRBO;//深度渲染缓冲附件
	VAOID mBloomVAO;
	VBOID mBloomVBO;
	Texture2DID mBloomTex[2];

	FBOID mPingPongFBO[2];
	Texture2DID mPingPongTex[2];

	static bool bIsCreatedPPMesh;

private:
	//天空盒子网格和着色器

	ShaderID mSkyboxShader;
	unsigned mSkyboxTexture;
	VAOID mSkyboxVAO;
	VBOID mSkyboxVBO;
	int mSkyboxVerticesNum;

private:
	//阴影映射着色器和帧缓冲

	FBOID mShadowMapFBO;
	unsigned mShadowMapDepthTexture;
	unsigned mShadowMapWidth;
	unsigned mShadowMapHeight;
	ShaderID mShadowMapShaderID;
	vector<unsigned> mShadowMapDepthTextures;

	ShaderID mFullScreenShader;
private:
	//Canvas渲染资源的句柄
	ShaderID mCanvasShader;
	Texture2DID mCanvasTexture;
	VAOID mCanvasVAO;
		/**
		 * @brief 渲染深度贴图以进行阴影映射
		 */
		//void RenderDepthMap();
		/**
		 * @brief
		 */
		//void PostProcessRender();
		/**
		 * @brief 将场景渲染到多重采样帧缓冲中
		 * 绑定多重采样帧缓冲
		 * 渲染场景
		 *
		 */
		//void BeginOffScreenRendering();
		/**
		* @brief 将多重采样帧缓冲渲染的纹理转化为普通纹理
		*/
		//void EndOffScreenRendring();
	
	
};

