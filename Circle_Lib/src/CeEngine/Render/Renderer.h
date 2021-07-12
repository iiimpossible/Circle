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
	ERE_Bloom = 0,//����
	ERE_MultiSample_4,//���ز��������
	ERE_MultiSample_8,
	ERE_MultiSample_16,
	ERE_Reflect,//����
	ERE_Shadow,//��Ӱ
	ERE_Refraction,//����
	ERE_Gamma//��У��
};


 /**
 * @brief ��Ⱦ�����������Ⱦ��������ദ��ġ�
 * ��Ⱦ�߼�Ϊ�����붥�����ݰ���������ɫ��������
 * �������ݣ�����ɫ��id��transform�����material�����mesh�����
 * ��Ⱦϵͳÿһ�λ��Ƶ��ö���һ����Ⱦ�����Ⱦ�����ж����������ƣ��ж�άͼƬ���ƣ��ȵȡ�
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
	 * @brief ��Ⱦ������
	 * ֧����Ⱦ2Dģ�ͣ�3D������ģ��
	 * ֧�ֺ���Ч��
	 */
	void Update();

	/** @brief 2D��Ⱦ��*/
	void Render2D();
 
	/** @breif ���Ŀǰ��������Ⱦ��������볡�йص���ɫ����δ������������ȾUI��������ʹ��һ����������ȾUI*/
	void RenderCanvas();

	void InitRender();

	void Start();
	 
	/** @breif ����Ƕ�λ����ĳ��ԣ��������ر���ȾЧ��*/
	void SetRenderEffect(ERenderEffect aEffect);
public:
	//��Ⱦ����
	 
	/** @breif ����һ����������б���,�����̬�������ܷ�����ĳ�Ա�𣿾�̬����ֻ�ܷ��ʾ�̬���������ܷ������Ա����*/	 
	static void InsertRenderCommand(RenderCommand* aRCmd);

	static void InsertRenderCommand(RenderCommand3D* aRCmd);

	static void InsertRenderCommand(RenderCommand2D* aRCmd);
	 
	/** @breif ���������δ��֧�ֶ������Ⱦ�Ĺؼ��������Ϣ��Ϊ�˱���ÿ����Ϸ���嶼Ҫ���������Ϣ*/	
	static void InsertCameraInfo(const CameraInfo& aInfo);
	
public:
	//��������

	static void InsertLightCommand(const LightCommand& aLCmd);
private:

	/**
	 * @brief ������ɫ������
	 * @details ģ�;����ӿھ���ͶӰ����
	 */
	 void SetShader(const unsigned int aShaderHandle, const RenderCommand* aRC);
	/**
	 * @brief 2D��ɫ������
	 * @details
	 ģ�;��� ���������ƶ�
	 �ӿھ��� ������ƶ�
	 ͶӰ���� ͼ��ӳ��
	 */
	 void SetShader2D(const RenderCommand2D* aRC);
	 void SetTexttue2D(const RenderCommand2D* aRC);

	/** @breif ��ÿ����ɫ���Ĺ�����Ϣ��ֵ*/
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

	//���Ҫʵ�ֻ����ѺõĻ�����ô��Ӧ�����ⲿʹ��һ���࣬��Ϊ������תվ������Ⱦ������һ������洢����
	static vector<RenderCommand3D*> mRC3DContainer;

	//Ŀǰ����Ҫ��������ʹ������
	static vector<RenderCommand2D*> mRC2DContainer;

	static vector<LightCommand> mLightCmds;

	static CameraInfo mCameraInfo;
private:
	
	//�Ƿ����ú���
	bool bPostProcess;
	//�Ƿ�������Ȳ���
	bool bEnableDepthTest;
	//�Ƿ�����ģ�����
	bool bEnableStencilTest;
	//�Ƿ�3D����
	bool bIs3DScene;
	//�Ƿ����÷���
	bool bEnableBloom;
	//�Ƿ����ÿ����
	bool bEnableMultiSample;	

private:
	//���ز������֡����
	FBOID mMSFBO;	
	Texture2DID mMSBuffer[2];
	Texture2DID mMSDepthbuffer;

	//�н�֡����
	FBOID mIntermediateFBO;
	Texture2DID mIntermediateBuffer;

private:
	//����Ч�����֡����

	ShaderID mBloomShader;//������ɫ�� Ҳ���Բ����Ƿ�ɹ��������֡����
	ShaderID mBlurShader;//��˹ģ����ɫ��
	ShaderID mBloomFinalShader;//����������ɫ��	
	FBOID mBloomFBO;//����֡����		
	RBOID mBloomDepthRBO;//�����Ⱦ���帽��
	VAOID mBloomVAO;
	VBOID mBloomVBO;
	Texture2DID mBloomTex[2];

	FBOID mPingPongFBO[2];
	Texture2DID mPingPongTex[2];

	static bool bIsCreatedPPMesh;

private:
	//��պ����������ɫ��

	ShaderID mSkyboxShader;
	unsigned mSkyboxTexture;
	VAOID mSkyboxVAO;
	VBOID mSkyboxVBO;
	int mSkyboxVerticesNum;

private:
	//��Ӱӳ����ɫ����֡����

	FBOID mShadowMapFBO;
	unsigned mShadowMapDepthTexture;
	unsigned mShadowMapWidth;
	unsigned mShadowMapHeight;
	ShaderID mShadowMapShaderID;
	vector<unsigned> mShadowMapDepthTextures;

	ShaderID mFullScreenShader;
private:
	//Canvas��Ⱦ��Դ�ľ��
	ShaderID mCanvasShader;
	Texture2DID mCanvasTexture;
	VAOID mCanvasVAO;
		/**
		 * @brief ��Ⱦ�����ͼ�Խ�����Ӱӳ��
		 */
		//void RenderDepthMap();
		/**
		 * @brief
		 */
		//void PostProcessRender();
		/**
		 * @brief ��������Ⱦ�����ز���֡������
		 * �󶨶��ز���֡����
		 * ��Ⱦ����
		 *
		 */
		//void BeginOffScreenRendering();
		/**
		* @brief �����ز���֡������Ⱦ������ת��Ϊ��ͨ����
		*/
		//void EndOffScreenRendring();
	
	
};

