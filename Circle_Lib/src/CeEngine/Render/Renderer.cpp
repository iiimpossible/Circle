#include <CeCommon/OpenGL/CeDraw.h>
#include <CeCommon/OpenGL/CeShaderCompiler.h>
#include <CeCommon/OpenGL/CeMeshFormatter.h>
#include <CeCommon/OpenGL/CeWindow.h>
#include <CeCommon/Tool/CeLoader.h>
#include <CeCommon/Tool/Time.h>
#include <CeEngine/Manager/DebugLogManager.h>
#include <CeEngine/Manager/ResourceManager.h>
#include <CeEngine/Manager/SceneManager.h>
#include <CeEngine/Data/InlineData.h>
#include <CeEngine/Engine.h>


#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Mesh.h" 
#include "Texture2D.h"

list<RenderCommand*> Renderer::mRenderCmds;
vector<RenderCommand3D*> Renderer::mRC3DContainer;
vector<RenderCommand2D*> Renderer::mRC2DContainer;
vector<LightCommand> Renderer::mLightCmds; 
bool Renderer::bIsCreatedPPMesh = false;
CameraInfo Renderer::mCameraInfo;

/**
* @brief ������Ⱦ��ʼ��
* ����һ�����ز���֡����
* ����һ�������н�֡����
* ����ѭ���н�������Ⱦ�����ز���֡����
* �����ز���֡�����е�ֵ���Ƶ��н�֡�����У�����ֱ��ʹ�ö��ز���֡������Ⱦ������
* ��Ĭ��֡����
* ���н�֡�����������Ⱦ����Ļ�������о���任����ȫ�ڱ�׼�豸����ϵ�ϣ���Ⱦ��һ��ռ��������Ļ��Quad��
*/
Renderer::Renderer(){}
Renderer::~Renderer(){}


void Renderer::Start()
{
	string bloom_vshader = CeLoader::LoadText("asset/material/shader/bloom_v.shader");
	string bloom_fshader = CeLoader::LoadText("asset/material/shader/bloom_f.shader");
	mBloomShader = CeShaderCompiler::CompileShader(bloom_vshader, bloom_fshader);
	string bloom_final_vshader = CeLoader::LoadText("asset/material/shader/bloom_final_v.shader");
	string bloom_final_fshader = CeLoader::LoadText("asset/material/shader/bloom_final_f.shader");
	mBloomFinalShader = CeShaderCompiler::CompileShader(bloom_final_vshader, bloom_final_fshader);
	string bler_vshader = CeLoader::LoadText("asset/material/shader/blur_v.shader");
	string bler_fshader = CeLoader::LoadText("asset/material/shader/blur_f.shader");
	mBlurShader = CeShaderCompiler::CompileShader(bler_vshader, bler_fshader);
	//mBloomShader = CeShaderCompiler::CompileShader(vertex_shader, fragment_shader);
	

	////����������
	bPostProcess = false;
	bEnableDepthTest = false;
	bEnableStencilTest = false;
	bIs3DScene = false;
	bEnableBloom = true;
	bEnableMultiSample = false;

	//��������
	HDRFramebufferParam hdr_param;
	CeTextureFormatter::CreateHRDFramebuffer(hdr_param);
	mBloomFBO = hdr_param.hdr_fbo;
	mBloomDepthRBO = hdr_param.hdr_depthbuffer;
	mBloomTex[0] = hdr_param.hdr_colorbuffer[0];
	mBloomTex[1] = hdr_param.hdr_colorbuffer[1];
	mPingPongFBO[0] = hdr_param.pingpong_fbo[0];
	mPingPongFBO[1] = hdr_param.pingpong_fbo[1];
	mPingPongTex[0] = hdr_param.pingpong_colorbuffer[0];
	mPingPongTex[1] = hdr_param.pingpong_colorbuffer[1];

	//���ز�������
	MSFramebufferParam ms_param;
	ms_param.sample = 4;//��������
	CeTextureFormatter::CreateMSFramebuffer(ms_param);
	mMSFBO = ms_param.ms_fbo;
	mMSBuffer[0] = ms_param.ms_colorbuffer1;
	mMSBuffer[1] = ms_param.ms_colorbuffer2;

	ColorFramebufferParam color_param;
	CeTextureFormatter::CreateColorFramebuffer(color_param);
	mIntermediateBuffer = color_param.color_buffer;
	mIntermediateFBO = color_param.color_fbo;

	//��պ�������
	Shader skybox_shader = ResourceManager::GetShader("Pre_Skybox");
	mSkyboxShader = skybox_shader.mShaderID;
	mSkyboxVAO = skybox_shader.mVAO;
	mSkyboxVBO = skybox_shader.mVBO;
	mSkyboxTexture = skybox_shader.mTextureID;
	mSkyboxVerticesNum = skybox_shader.mVerticesNum;

	//��Ӱӳ������ 
	mShadowMapShaderID = CeShaderCompiler::CompileShader(CeLoader::LoadText("asset/material/shader/shadowmap_v.shader"),
		CeLoader::LoadText("asset/material/shader/shadowmap_f.shader"));
	FBOParam shadowmap_param;
	shadowmap_param.width = 1024;
	shadowmap_param.height = 1024;
	CeTextureFormatter::CreateFrambufferWithDepthTexture(shadowmap_param,mShadowMapDepthTexture);
	mShadowMapFBO = shadowmap_param.fbo;
	mShadowMapWidth = shadowmap_param.width;
	mShadowMapHeight = shadowmap_param.height;

	//��ͨ������ȫ����Ⱦ��ɫ��
	mFullScreenShader = CeShaderCompiler::CompileShader(CeLoader::LoadText("asset/material/shader/fullscreen_v.shader"),
		CeLoader::LoadText("asset/material/shader/fullscreen_f.shader"));

}


void Renderer::InsertRenderCommand(RenderCommand* aRC)
{
		mRenderCmds.push_back(aRC);
}

void Renderer::InsertRenderCommand(RenderCommand3D * aRC3D)
{
	mRC3DContainer.push_back(aRC3D);
}

void Renderer::InsertRenderCommand(RenderCommand2D * aRC2D)
{
	mRC2DContainer.push_back(aRC2D);//���ﲢ����Ҫ�п�
}

void Renderer::InsertCameraInfo(const CameraInfo & aInfo)
{
	mCameraInfo = aInfo;
}

void Renderer::InsertLightCommand(const LightCommand & aLCmd)
{
	mLightCmds.push_back(aLCmd);
}

/*===========================================================================================================*/
void Renderer::SetShader(const unsigned int aShaderHd, const RenderCommand* aRCmd)
{
	if (!aRCmd)return;
	///������ɫ��
	//CeShaderCompiler::UseShaderProgram(mShadowMapShaderID);

	CeShaderCompiler::UseShaderProgram(aShaderHd);
	///���ü��α任
	Mat4 model = Mat4(1.0f);
	//Mat4 view = Mat4(1.0f);
	//Mat4 projection = Mat4(1.0f);
	model = CeTranslate(model, aRCmd->mPos);
	model = CeRotate(model, CeRadians(aRCmd->mRotation.x), Vec3(1.0f, 0.0f, 0.0f));
	model = CeRotate(model, CeRadians(aRCmd->mRotation.y), Vec3(0.0f, 1.0f, 0.0f));
	model = CeRotate(model, CeRadians(aRCmd->mRotation.z), Vec3(0.0f, 0.0f, 1.0f));	 

	model = CeScale(model, aRCmd->mSize);
	/////ֻ��Ҫ�������λ�ú�ǰ������������������������
	//view = CeLoolAt(aRCmd->mCPos, aRCmd->mCPos + aRCmd->mCFront, Vec3(0.0f, 1.0f, 0.0f));	 
	 
	//projection = aRCmd->mProjMat;
	//DebugLogManager::DebugOutputMat4("Renderer", projection);
	///�任�������
	CeShaderCompiler::SetVec3(aShaderHd, "iCameraPos", mCameraInfo.cameraPos);
	CeShaderCompiler::SetMat4(aShaderHd,"iModel",model);
	CeShaderCompiler::SetMat4(aShaderHd, "iView", mCameraInfo.view);
	CeShaderCompiler::SetMat4(aShaderHd, "iProjection", mCameraInfo.projection);
	CeShaderCompiler::SetFloat(aShaderHd, "iTime", CeTimer::GetTime());

	//���ղ���	
	CeShaderCompiler::SetVec3(aShaderHd, "iAmbientColor", Vec3(0.2f));
	CeShaderCompiler::SetInt(aShaderHd, "iLightNum", (int)mLightCmds.size());//���ù�Դ������

	CeTextureFormatter::BindTexture2D(aShaderHd,mShadowMapDepthTexture);
	//��Ӱ
	if (mLightCmds.size() > 0)
	{
		CeShaderCompiler::SetMat4(aShaderHd, "iLightView", mLightCmds.front().lightView);
		CeShaderCompiler::SetMat4(aShaderHd, "iLightProj", mLightCmds.front().lightProjection);
	}
	
	
	//CeShaderCompiler::SetVec3(aShaderHandle, "");
	//�������������Ŀǰ����10��
	SetShaderLight(aShaderHd);
}


/*===========================================================================================================*/


void Renderer::SetShader2D(const RenderCommand2D * aRCmd)
{
	if (!aRCmd) return;
	unsigned int tShaderHD = aRCmd->mShaderHandle;
	CeShaderCompiler::UseShaderProgram(tShaderHD); 
	Mat4 model = Mat4(1.0f);
	Mat4 projection = Mat4(1.0f);
	Mat4 view = Mat4(1.0f);
	Vec3 t = Vec3(0.0f, 0.0f, 1.0f);	
	view = aRCmd->mViewMat;
	view[0][0] = 1.0f;
	view[1][1] = 1.0f;
	view[2][2] = 1.0f;

	view[0][2] = 0.0f;
	view[1][2] = 0.0f;
	view[2][2] = 0.0f;
	view[3][2] = 0.0f;
	
	projection = aRCmd->mProjectionMat;
	model = CeTranslate(model, Vec3(aRCmd->mPosition,0.0f) );
	model = CeRotate(model, CeRadians(aRCmd->mRotation.x), Vec3(0.0f, 0.0f, 1.0f));
	model = CeScale(model, Vec3(aRCmd->mSize, 1.0f));

	CeShaderCompiler::SetMat4(tShaderHD, "iModel", model);
	CeShaderCompiler::SetMat4(tShaderHD, "iView", view);
	CeShaderCompiler::SetMat4(tShaderHD, "iProjection", projection);
	CeShaderCompiler::SetFloat(tShaderHD, "iTime", CeTimer::GetTime());//iǰ׺λinput
	
	//���ʱ���������λ�ã�ʱ�䣬��

}


/*===========================================================================================================*/


void Renderer::SetTexttue2D(const RenderCommand2D* aRCmd)
{
	if (!aRCmd) return;
	CeTextureFormatter::BindTexture2D(aRCmd->mShaderHandle, aRCmd->mSpriteHandle); 

}

void Renderer::SetShaderLight(unsigned aShaderHd)
{
	//����Դɾ���󣬱�������ɫ���е���ֵû�й��㣬��ˣ���ɾ����Դ����Ȼ�й��ա�
	for (int i = 0;i<mLightCmds.size() && i <10;i++)
	{
		CeShaderCompiler::SetVec3(aShaderHd, "PointLights[" + to_string(i) + "].color", mLightCmds[i].lightColor);
		CeShaderCompiler::SetVec3(aShaderHd, "PointLights[" + to_string(i) + "].position", mLightCmds[i].lightPos);
	}

}

void Renderer::DrawFrameBuffer()
{
	CeTextureFormatter::BindFramebuffer(mBloomFBO);
	unsigned  attachment[2]	{EColorAttachment::ECA_0,EColorAttachment::ECA_1};
	CeGraphDrawing::DrawFramebuffer(2, attachment);
	CeTextureFormatter::UnbindFrameBuffer();
}

void Renderer::DrawSkybox()
{
	CeGraphWindow::SetViewport(CeGraphWindow::GetWindowSize());
	//�ر����ֵд��
	CeGraphWindow::DisableDepthWrite();
	CeShaderCompiler::UseShaderProgram(mSkyboxShader);
	CeShaderCompiler::SetMat4(mSkyboxShader, "iProjection", mCameraInfo.projection);
	CeShaderCompiler::SetMat4(mSkyboxShader, "iView", Mat4(Mat3(mCameraInfo.view)));
	CeGraphDrawing::DrawSkybox(mSkyboxShader, mSkyboxVAO, mSkyboxTexture, mSkyboxVerticesNum);
	//�������ֵд��
	CeGraphWindow::EnableDepthWrite();

}

void Renderer::DrawScene()
{
	CeGraphWindow::SetViewport(CeGraphWindow::GetWindowSize());
	for (auto rcmd : mRenderCmds)
	{
		if (rcmd->bIgnore)
		{
			delete rcmd;
			continue;
		}
		if (rcmd->mshaderElements.size() == 1)
		{
			SetShader(rcmd->mshaderElements[0].shader_hd, rcmd);

			unsigned texname_offset = CeShaderCompiler::SetTextureUnitName(rcmd->mshaderElements[0].shader_hd, "iShadowMap");
			CeShaderCompiler::SetTextureUnitName(rcmd->mshaderElements[0].shader_hd,
				rcmd->mshaderElements[0].texture_name, texname_offset);

			//����Ӱ��ͼ����������Ԫƫ��
			unsigned tex_unitoffset = 
				CeTextureFormatter::BindTexture2D(rcmd->mshaderElements[0].shader_hd, mShadowMapDepthTexture);

			if (rcmd->mshaderElements[0].texture_hd.size() > 0)
			{
				CeTextureFormatter::BindTexture2D(rcmd->mshaderElements[0].shader_hd,
					rcmd->mshaderElements[0].texture_hd,tex_unitoffset);
			}
			for (int i = 0; i < rcmd->mVAOIDs.size(); i++)
			{
				CeGraphDrawing::DrawGraphWithIndice(rcmd->mVAOIDs[i], rcmd->mIndicesNum[i]);
			}

		}
		else if (rcmd->mshaderElements.size() > 1)
		{
			//���ܵĴ�����Ϊ�е�ģ�͵������ǵ����ģ������۾�����������ʱ��������������ɫ������
			for (int i = 0; i < rcmd->mshaderElements.size() && i < rcmd->mVAOIDs.size(); i++)
			{
				SetShader(rcmd->mshaderElements[i].shader_hd, rcmd);

				
				unsigned texname_offset = CeShaderCompiler::SetTextureUnitName(rcmd->mshaderElements[i].shader_hd,"iShadowMap");

				CeShaderCompiler::SetTextureUnitName(rcmd->mshaderElements[i].shader_hd, 
					rcmd->mshaderElements[i].texture_name, texname_offset);

				//����Ӱ��ͼ����������Ԫƫ��
				unsigned tex_unitoffset =
					CeTextureFormatter::BindTexture2D(rcmd->mshaderElements[i].shader_hd, mShadowMapDepthTexture);
				
				CeTextureFormatter::BindTexture2D(rcmd->mshaderElements[i].shader_hd,
					rcmd->mshaderElements[i].texture_hd, tex_unitoffset);
				CeGraphDrawing::DrawGraphWithIndice(rcmd->mVAOIDs[i], rcmd->mIndicesNum[i]);
			}
		}
		delete rcmd;//�ͷ���Ⱦ������ڴ�
	}

}

/** @breif ÿ����Դ����Ҫ��Ⱦһ�������ͼ...*/
void Renderer::DrawShadowMaps()
{
	CeGraphWindow::EnableTest(EEnalbe::EEB_DepthTest);
	if (mLightCmds.empty()) return;
	//�󶨵���Ӱӳ��֡����
	CeTextureFormatter::BindFramebuffer(mShadowMapFBO, mShadowMapWidth, mShadowMapHeight);
	CeTextureFormatter::ClearDepthbuffer();//�����Ȼ���
	CeShaderCompiler::UseShaderProgram(mShadowMapShaderID);//ʹ����Ӱӳ����ɫ��
	//CeShaderCompiler::SetMat4(mShadowMapShaderID,"iLightSpace",mLightCmds.front().lightSpaceMat);//��ռ�任����ortho*view
	
 
	CeShaderCompiler::SetMat4(mShadowMapShaderID, "iView", mLightCmds.front().lightView);
	CeShaderCompiler::SetMat4(mShadowMapShaderID, "iProjection", mLightCmds.front().lightProjection);

	 
	
	//��Ⱦ�����ͼ����Ҫ�����������Ҫ������ӿھ�����Ϊ���ڹ�Դ���ӿڣ���Ҫģ�;��󽫶�������ת����ģ�Ϳռ�
	for (auto rcmd : mRenderCmds)
	{
		if (rcmd->mshaderElements.size() == 1)
		{	
			Mat4 model(1.0f);  
			model = CeTranslate(model, rcmd->mPos);
			model = CeRotate(model, CeRadians(rcmd->mRotation.x), Vec3(1.0f, 0.0f, 0.0f));
			model = CeRotate(model, CeRadians(rcmd->mRotation.y), Vec3(0.0f, 1.0f, 0.0f));
			model = CeRotate(model, CeRadians(rcmd->mRotation.z), Vec3(0.0f, 0.0f, 1.0f));
			model = CeScale(model, rcmd->mSize);
			CeShaderCompiler::SetMat4(mShadowMapShaderID, "iModel", model);		
			for (int i = 0; i < rcmd->mVAOIDs.size(); i++)
			{
				CeGraphDrawing::DrawGraphWithIndice(rcmd->mVAOIDs[i], rcmd->mIndicesNum[i]);
			}
		}
		else if (rcmd->mshaderElements.size() > 1)
		{
			//���ܵĴ�����Ϊ�е�ģ�͵������ǵ����ģ������۾�����������ʱ��������������ɫ������
			for (int i = 0; i < rcmd->mshaderElements.size() && i < rcmd->mVAOIDs.size(); i++)
			{			 
				Mat4 model(1.0f);//֮ǰ�������û�г�ʼ�����¾������ݴ���Mat4 model =  CeTranslate(model, rcmd->mPos);
				model = CeTranslate(model, rcmd->mPos);
				model = CeRotate(model, CeRadians(rcmd->mRotation.x), Vec3(1.0f, 0.0f, 0.0f));
				model = CeRotate(model, CeRadians(rcmd->mRotation.y), Vec3(0.0f, 1.0f, 0.0f));
				model = CeRotate(model, CeRadians(rcmd->mRotation.z), Vec3(0.0f, 0.0f, 1.0f));
				model = CeScale(model, rcmd->mSize);
				CeShaderCompiler::SetMat4(mShadowMapShaderID, "iModel", model);	 
				CeGraphDrawing::DrawGraphWithIndice(rcmd->mVAOIDs[i], rcmd->mIndicesNum[i]);
			}
		}
		//delete rcmd;
	}
	CeTextureFormatter::UnbindFrameBuffer();
	/*CeGraphWindow::SetViewport(CeGraphWindow::GetWindowSize());
	CeShaderCompiler::UseShaderProgram(mFullScreenShader);
	CeTextureFormatter::BindTexture2D(mFullScreenShader, mShadowMapDepthTexture,"iShadowMap");
	CeGraphDrawing::DrawFullScreen();*/

}

void Renderer::StartBloom()
{
	//������ز���֡�������������ز�����ɫ������HDR����һ����Ȼ��帽����Ŀǰû��ģ�建��
	CeTextureFormatter::BindFramebuffer(mMSFBO);//��Ⱦ�����ز���֡����
	CeGraphWindow::SetViewport(800, 600);//����Ϊ��ɫ�������õķֱ���
	CeGraphDrawing::ClearBuffer();
}

void Renderer::EndBloom()
{
	//����������Ⱦ��������Ⱦ����ͼ��
	CeTextureFormatter::UnbindFrameBuffer();
	//�����ز���֡�����2����ɫ�������Ƶ���ͨ֡����
	CeTextureFormatter::ExchangeColorbuffer(mMSFBO, mBloomFBO, 800, 600, 2);//����������

	//��ʼ���и�˹ģ��
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	CeShaderCompiler::UseShaderProgram(mBlurShader);//ʹ�ø�˹ģ����ɫ��
	for (unsigned int i = 0; i < amount; i++)
	{
		//�󶨵�һ��pingpong֡����
		CeTextureFormatter::BindFramebuffer(mPingPongFBO[horizontal]);
		CeGraphWindow::SetViewport(800, 600);//����Ϊ��ɫ�������õķֱ���
		CeShaderCompiler::SetBool(mBlurShader, "iHorizontal", horizontal);
		//������һ��pingpong��ɫ������д�������ɫ����
		CeShaderCompiler::SetTextureUnitName(mBlurShader, "iImage");
		CeTextureFormatter::BindTexture2D(mBlurShader, first_iteration ? mBloomTex[1] : mPingPongTex[!horizontal],0);// bind texture of other framebuffer (or scene if first iteration)

		//���и�˹ģ����Ⱦ
		CeGraphDrawing::DrawFullScreen();

		horizontal = !horizontal;//�����˹ģ���л��ݺ�
		if (first_iteration)
			first_iteration = false;
	}

	CeTextureFormatter::UnbindFrameBuffer();//����˹ģ��֡����		
	CeGraphWindow::SetViewport(CeGraphWindow::GetWindowSize());//���õ�ǰ�ӿڵ���ʾ����		

	//�������յķ�����Ⱦ
	CeShaderCompiler::UseShaderProgram(mBloomFinalShader);	
	CeShaderCompiler::SetTextureUnitName(mBloomFinalShader, vector<string>{ "iScene", "iBloom" },0);
	CeTextureFormatter::BindTexture2D(mBloomFinalShader, { mBloomTex[0],mPingPongTex[!horizontal] });
	CeGraphDrawing::DrawFullScreen();
}


/*===========================================================================================================*/



void Renderer::Update()
{
	if (mRenderCmds.empty())
	{
		//����Ⱦ����Ϊ�յ�����»�����պ���
		//DrawSkybox();
		return;
	}
	 	//���⿪��
	if (false)//bEnableBloom
	{			//��HDR֡���壬
		CeTextureFormatter::BindFramebuffer(mBloomFBO);		
		CeGraphDrawing::ClearBuffer();//�������HDR֡�������
	}


	if (true)
	{
		//���������ͼ����Ӱӳ�䣩
		DrawShadowMaps();
	}

	if (true)//bEnableMultiSample
	{
		StartBloom();
	}	


	//�Ƿ���Ƴ���
	if (true)
	{
		//������պ���
		DrawSkybox();

		//ͨ����Ⱦ���������Ϸ����
		DrawScene();
	}	 

	//�������
	if (true)//bEnableBloom
	{
		EndBloom();
	}

	if (false)//bEnableMultiSample ������ز�����֧�ַ���
	{
		CeTextureFormatter::ExchangeColorbuffer(mMSFBO,mIntermediateFBO,800,600,2);
		CeTextureFormatter::UnbindFrameBuffer();
		CeShaderCompiler::UseShaderProgram(mBloomShader);
		CeShaderCompiler::SetTextureUnitName(mBloomShader, "iImage");
		CeTextureFormatter::BindTexture2D(mBloomShader, mIntermediateBuffer);
		CeGraphDrawing::DrawFullScreen();
	}
	mRenderCmds.clear();///<�����Ⱦ�б�
	mLightCmds.clear();
}


/*===========================================================================================================*/


void Renderer::Render2D()
{
	for (RenderCommand2D* rcd : mRC2DContainer)
	{		
		SetShader2D(rcd);
		SetTexttue2D(rcd);
		CeGraphDrawing::DrawGraph(rcd->mVAOHandle,6);
		delete rcd;//�ͷ���Ⱦ������ڴ�
	}
	mRC2DContainer.clear();
}



/*===========================================================================================================*/



void Renderer::RenderCanvas()
{
	CeShaderCompiler::UseShaderProgram(mCanvasShader); 
	Mat4 model = Mat4(1.0f);
	Mat4 view = Mat4(1.0f);
	Mat4 projection = Mat4(1.0f);

	model = CeTranslate(model, Vec3(400.0f, 300.0f, 0.0f));
	model = CeScale(model, Vec3(800.0f, 600.0f, 0.0f));

	projection = CeOrtho(0.0f, 800.0f, 600.0f, 0.0f);


	CeShaderCompiler::SetVec2(mCanvasShader, "iResolution", Vec2(800.0f, 600.0f));
	CeShaderCompiler::SetMat4(mCanvasShader, "model", model);
	CeShaderCompiler::SetMat4(mCanvasShader, "view", view);
	CeShaderCompiler::SetMat4(mCanvasShader, "projection", projection);
	CeShaderCompiler::SetFloat(mCanvasShader, "iTime", CeTimer::GetTime());
	CeGraphDrawing::DrawGraph(mCanvasVAO, 6);
}

void Renderer::InitRender()
{
	unsigned a = CeMeshFormatter::CreateVAO();//
}


/*===========================================================================================================*/



void Renderer::SetRenderEffect(ERenderEffect aEffect)
{

}


 