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
* @brief 离屏渲染初始化
* 创建一个多重采样帧缓冲
* 创建一个常规中介帧缓冲
* 在主循环中将场景渲染到多重采样帧缓冲
* 将多重采样帧缓冲中的值复制到中介帧缓冲中（不能直接使用多重采样帧缓冲渲染的纹理）
* 绑定默认帧缓冲
* 将中介帧缓冲的纹理渲染到屏幕（不进行矩阵变换，完全在标准设备坐标系上，渲染到一个占满整个屏幕的Quad上
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
	

	////后处理器控制
	bPostProcess = false;
	bEnableDepthTest = false;
	bEnableStencilTest = false;
	bIs3DScene = false;
	bEnableBloom = true;
	bEnableMultiSample = false;

	//泛光设置
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

	//多重采样设置
	MSFramebufferParam ms_param;
	ms_param.sample = 4;//采样数量
	CeTextureFormatter::CreateMSFramebuffer(ms_param);
	mMSFBO = ms_param.ms_fbo;
	mMSBuffer[0] = ms_param.ms_colorbuffer1;
	mMSBuffer[1] = ms_param.ms_colorbuffer2;

	ColorFramebufferParam color_param;
	CeTextureFormatter::CreateColorFramebuffer(color_param);
	mIntermediateBuffer = color_param.color_buffer;
	mIntermediateFBO = color_param.color_fbo;

	//天空盒子数据
	Shader skybox_shader = ResourceManager::GetShader("Pre_Skybox");
	mSkyboxShader = skybox_shader.mShaderID;
	mSkyboxVAO = skybox_shader.mVAO;
	mSkyboxVBO = skybox_shader.mVBO;
	mSkyboxTexture = skybox_shader.mTextureID;
	mSkyboxVerticesNum = skybox_shader.mVerticesNum;

	//阴影映射数据 
	mShadowMapShaderID = CeShaderCompiler::CompileShader(CeLoader::LoadText("asset/material/shader/shadowmap_v.shader"),
		CeLoader::LoadText("asset/material/shader/shadowmap_f.shader"));
	FBOParam shadowmap_param;
	shadowmap_param.width = 1024;
	shadowmap_param.height = 1024;
	CeTextureFormatter::CreateFrambufferWithDepthTexture(shadowmap_param,mShadowMapDepthTexture);
	mShadowMapFBO = shadowmap_param.fbo;
	mShadowMapWidth = shadowmap_param.width;
	mShadowMapHeight = shadowmap_param.height;

	//普通的纹理全屏渲染着色器
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
	mRC2DContainer.push_back(aRC2D);//这里并不需要判空
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
	///启用着色器
	//CeShaderCompiler::UseShaderProgram(mShadowMapShaderID);

	CeShaderCompiler::UseShaderProgram(aShaderHd);
	///配置几何变换
	Mat4 model = Mat4(1.0f);
	//Mat4 view = Mat4(1.0f);
	//Mat4 projection = Mat4(1.0f);
	model = CeTranslate(model, aRCmd->mPos);
	model = CeRotate(model, CeRadians(aRCmd->mRotation.x), Vec3(1.0f, 0.0f, 0.0f));
	model = CeRotate(model, CeRadians(aRCmd->mRotation.y), Vec3(0.0f, 1.0f, 0.0f));
	model = CeRotate(model, CeRadians(aRCmd->mRotation.z), Vec3(0.0f, 0.0f, 1.0f));	 

	model = CeScale(model, aRCmd->mSize);
	/////只需要摄像机的位置和前向量，上向量是世界上向量
	//view = CeLoolAt(aRCmd->mCPos, aRCmd->mCPos + aRCmd->mCFront, Vec3(0.0f, 1.0f, 0.0f));	 
	 
	//projection = aRCmd->mProjMat;
	//DebugLogManager::DebugOutputMat4("Renderer", projection);
	///变换矩阵更新
	CeShaderCompiler::SetVec3(aShaderHd, "iCameraPos", mCameraInfo.cameraPos);
	CeShaderCompiler::SetMat4(aShaderHd,"iModel",model);
	CeShaderCompiler::SetMat4(aShaderHd, "iView", mCameraInfo.view);
	CeShaderCompiler::SetMat4(aShaderHd, "iProjection", mCameraInfo.projection);
	CeShaderCompiler::SetFloat(aShaderHd, "iTime", CeTimer::GetTime());

	//光照测试	
	CeShaderCompiler::SetVec3(aShaderHd, "iAmbientColor", Vec3(0.2f));
	CeShaderCompiler::SetInt(aShaderHd, "iLightNum", (int)mLightCmds.size());//设置光源的数量

	CeTextureFormatter::BindTexture2D(aShaderHd,mShadowMapDepthTexture);
	//阴影
	if (mLightCmds.size() > 0)
	{
		CeShaderCompiler::SetMat4(aShaderHd, "iLightView", mLightCmds.front().lightView);
		CeShaderCompiler::SetMat4(aShaderHd, "iLightProj", mLightCmds.front().lightProjection);
	}
	
	
	//CeShaderCompiler::SetVec3(aShaderHandle, "");
	//遍历光照组件，目前上限10个
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
	CeShaderCompiler::SetFloat(tShaderHD, "iTime", CeTimer::GetTime());//i前缀位input
	
	//材质变量：世界位置，时间，等

}


/*===========================================================================================================*/


void Renderer::SetTexttue2D(const RenderCommand2D* aRCmd)
{
	if (!aRCmd) return;
	CeTextureFormatter::BindTexture2D(aRCmd->mShaderHandle, aRCmd->mSpriteHandle); 

}

void Renderer::SetShaderLight(unsigned aShaderHd)
{
	//当光源删除后，保存在着色器中的数值没有归零，因此，在删除光源后依然有光照。
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
	//关闭深度值写入
	CeGraphWindow::DisableDepthWrite();
	CeShaderCompiler::UseShaderProgram(mSkyboxShader);
	CeShaderCompiler::SetMat4(mSkyboxShader, "iProjection", mCameraInfo.projection);
	CeShaderCompiler::SetMat4(mSkyboxShader, "iView", Mat4(Mat3(mCameraInfo.view)));
	CeGraphDrawing::DrawSkybox(mSkyboxShader, mSkyboxVAO, mSkyboxTexture, mSkyboxVerticesNum);
	//开启深度值写入
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

			//绑定阴影贴图并返回纹理单元偏移
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
			//可能的错误：因为有的模型的网格不是单个的，比如眼睛有两个，这时网格数量大于着色器数量
			for (int i = 0; i < rcmd->mshaderElements.size() && i < rcmd->mVAOIDs.size(); i++)
			{
				SetShader(rcmd->mshaderElements[i].shader_hd, rcmd);

				
				unsigned texname_offset = CeShaderCompiler::SetTextureUnitName(rcmd->mshaderElements[i].shader_hd,"iShadowMap");

				CeShaderCompiler::SetTextureUnitName(rcmd->mshaderElements[i].shader_hd, 
					rcmd->mshaderElements[i].texture_name, texname_offset);

				//绑定阴影贴图并返回纹理单元偏移
				unsigned tex_unitoffset =
					CeTextureFormatter::BindTexture2D(rcmd->mshaderElements[i].shader_hd, mShadowMapDepthTexture);
				
				CeTextureFormatter::BindTexture2D(rcmd->mshaderElements[i].shader_hd,
					rcmd->mshaderElements[i].texture_hd, tex_unitoffset);
				CeGraphDrawing::DrawGraphWithIndice(rcmd->mVAOIDs[i], rcmd->mIndicesNum[i]);
			}
		}
		delete rcmd;//释放渲染命令的内存
	}

}

/** @breif 每个光源都需要渲染一个深度贴图...*/
void Renderer::DrawShadowMaps()
{
	CeGraphWindow::EnableTest(EEnalbe::EEB_DepthTest);
	if (mLightCmds.empty()) return;
	//绑定到阴影映射帧缓冲
	CeTextureFormatter::BindFramebuffer(mShadowMapFBO, mShadowMapWidth, mShadowMapHeight);
	CeTextureFormatter::ClearDepthbuffer();//清除深度缓冲
	CeShaderCompiler::UseShaderProgram(mShadowMapShaderID);//使用阴影映射着色器
	//CeShaderCompiler::SetMat4(mShadowMapShaderID,"iLightSpace",mLightCmds.front().lightSpaceMat);//光空间变换矩阵ortho*view
	
 
	CeShaderCompiler::SetMat4(mShadowMapShaderID, "iView", mLightCmds.front().lightView);
	CeShaderCompiler::SetMat4(mShadowMapShaderID, "iProjection", mLightCmds.front().lightProjection);

	 
	
	//渲染深度贴图不需要相机，即不需要相机的视口矩阵因为是在光源的视口，需要模型矩阵将顶点正常转换到模型空间
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
			//可能的错误：因为有的模型的网格不是单个的，比如眼睛有两个，这时网格数量大于着色器数量
			for (int i = 0; i < rcmd->mshaderElements.size() && i < rcmd->mVAOIDs.size(); i++)
			{			 
				Mat4 model(1.0f);//之前这个矩阵没有初始化导致矩阵数据错误，Mat4 model =  CeTranslate(model, rcmd->mPos);
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
	//这个多重采样帧缓冲有两个多重采样颜色附件（HDR）和一个深度缓冲附件，目前没有模板缓冲
	CeTextureFormatter::BindFramebuffer(mMSFBO);//渲染到多重采样帧缓冲
	CeGraphWindow::SetViewport(800, 600);//调整为颜色附件设置的分辨率
	CeGraphDrawing::ClearBuffer();
}

void Renderer::EndBloom()
{
	//结束离屏渲染并开启渲染后处理图像
	CeTextureFormatter::UnbindFrameBuffer();
	//将多重采样帧缓冲的2个颜色附件复制到普通帧缓冲
	CeTextureFormatter::ExchangeColorbuffer(mMSFBO, mBloomFBO, 800, 600, 2);//交换缓冲区

	//开始进行高斯模糊
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	CeShaderCompiler::UseShaderProgram(mBlurShader);//使用高斯模糊着色器
	for (unsigned int i = 0; i < amount; i++)
	{
		//绑定到一个pingpong帧缓冲
		CeTextureFormatter::BindFramebuffer(mPingPongFBO[horizontal]);
		CeGraphWindow::SetViewport(800, 600);//调整为颜色附件设置的分辨率
		CeShaderCompiler::SetBool(mBlurShader, "iHorizontal", horizontal);
		//向其中一个pingpong颜色缓冲中写入高亮颜色纹理
		CeShaderCompiler::SetTextureUnitName(mBlurShader, "iImage");
		CeTextureFormatter::BindTexture2D(mBlurShader, first_iteration ? mBloomTex[1] : mPingPongTex[!horizontal],0);// bind texture of other framebuffer (or scene if first iteration)

		//进行高斯模糊渲染
		CeGraphDrawing::DrawFullScreen();

		horizontal = !horizontal;//两遍高斯模糊切换纵横
		if (first_iteration)
			first_iteration = false;
	}

	CeTextureFormatter::UnbindFrameBuffer();//解绑高斯模糊帧缓冲		
	CeGraphWindow::SetViewport(CeGraphWindow::GetWindowSize());//设置当前视口的显示区域		

	//进行最终的泛光渲染
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
		//在渲染命令为空的情况下绘制天空盒子
		//DrawSkybox();
		return;
	}
	 	//泛光开启
	if (false)//bEnableBloom
	{			//绑定HDR帧缓冲，
		CeTextureFormatter::BindFramebuffer(mBloomFBO);		
		CeGraphDrawing::ClearBuffer();//将泛光的HDR帧缓冲清除
	}


	if (true)
	{
		//绘制深度贴图（阴影映射）
		DrawShadowMaps();
	}

	if (true)//bEnableMultiSample
	{
		StartBloom();
	}	


	//是否绘制场景
	if (true)
	{
		//绘制天空盒子
		DrawSkybox();

		//通过渲染命令绘制游戏物体
		DrawScene();
	}	 

	//泛光结束
	if (true)//bEnableBloom
	{
		EndBloom();
	}

	if (false)//bEnableMultiSample 这个多重采样不支持泛光
	{
		CeTextureFormatter::ExchangeColorbuffer(mMSFBO,mIntermediateFBO,800,600,2);
		CeTextureFormatter::UnbindFrameBuffer();
		CeShaderCompiler::UseShaderProgram(mBloomShader);
		CeShaderCompiler::SetTextureUnitName(mBloomShader, "iImage");
		CeTextureFormatter::BindTexture2D(mBloomShader, mIntermediateBuffer);
		CeGraphDrawing::DrawFullScreen();
	}
	mRenderCmds.clear();///<清空渲染列表
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
		delete rcd;//释放渲染命令的内存
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


 