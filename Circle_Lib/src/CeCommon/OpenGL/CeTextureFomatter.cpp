#include "CeTextureFomatter.h"
#include <gl/glew.h>
//#include <glad/glad.h>
#include <Celib_x64/src/CeEngine/Manager/DebugLogManager.h>
//
//#define GL_TEXTURE_2D 0x0DE1
//#define GL_TEXTURE_3D 0x806F
//#define GL_TEXTURE_CUBE_MAP 0x8513
//#define GL_TEXTURE_2D_ARRAY 0x8C1A
//#define GL_TEXTURE_CUBE_MAP_ARRAY_OES 0x9009
//#define GL_VIRTUAL_PAGE_SIZE_X_EXT 0x9195
//#define GL_VIRTUAL_PAGE_SIZE_Y_EXT 0x9196
//#define GL_VIRTUAL_PAGE_SIZE_Z_EXT 0x9197
//#define GL_MAX_SPARSE_TEXTURE_SIZE_EXT 0x9198
//#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_EXT 0x9199
//#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_EXT 0x919A
//#define GL_TEXTURE_SPARSE_EXT 0x91A6
//#define GL_VIRTUAL_PAGE_SIZE_INDEX_EXT 0x91A7
//#define GL_NUM_VIRTUAL_PAGE_SIZES_EXT 0x91A8
//#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_EXT 0x91A9
//#define GL_NUM_SPARSE_LEVELS_EXT 0x91AA
//


Texture2DInfo CeTextureFormatter::CreateTexture2D(unsigned char * aData, int aWidth, int aHeight, int aChannels,
	ETexAround aTextureAroundS,
	ETexAround aTextureAroundT,
	ETexFilter aTextureFilterMIN,
	ETexFilter aTextureFilterMGA)
{
	unsigned tex_hd = 0;	 
	glGenTextures(1, &tex_hd);
	glBindTexture(GL_TEXTURE_2D, tex_hd);

	if (aChannels == 4)
	{	//第二个参数是多级渐远纹理级别，0为基本级别。尺寸后边的0是历史遗留问题，都是0值
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aWidth, aHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, aData);
	}
	else if (aChannels == 3)	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aWidth, aHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, aData);
	else	//可能是灰度图，通道数量为1
		return {};
	
	glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//配置纹理参数，2D纹理类型，s方向，t方向环绕,如果是3D纹理还有一个r,与x,y,z等价。重复环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, aTextureAroundS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, aTextureAroundT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, aTextureFilterMIN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, aTextureFilterMGA);	
	glBindTexture(GL_TEXTURE_2D, 0);	
	//DebugLogManager::DebugOutput("CeTextureFormatter", EDebugTag::EDT_Log, "Create texture id is: " , (int)tex_hd);
	return Texture2DInfo{ tex_hd,aWidth ,aHeight ,aChannels };
}

unsigned CeTextureFormatter::CreateTextureCube(int aWidth, int aHeight, int aChannals, vector<unsigned char*>& aFaces)
{
	//安全性检测
	if (aFaces.size() != 6)
	{
		DebugLogManager::DebugOutput("CeTextureFormatter", EDebugTag::EDT_ERROR, { "Cube faces num is not 6." });
		return 0;
	}
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (unsigned int i = 0; i < aFaces.size(); i++)
	{		
		if (aChannals == 3)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, aWidth, aHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, aFaces[i]
			);
		}
		else if (aChannals == 4)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, aWidth, aHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, aFaces[i]
			);
		}
		
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}



/*===========================================================================================================*/



FBOID CeTextureFormatter::CreateFramebuffer()
{
	FBOID frambuffer;
	glGenFramebuffers(1, &frambuffer);	
	return frambuffer;
}

unsigned CeTextureFormatter::CreateTextureBuffer(TextureParam aTextureParam, int aAttachment)
{
	unsigned color_buffer;
	glGenTextures(1, &color_buffer);
	glBindTexture(GL_TEXTURE_2D, color_buffer);
	//注意当前视口的宽度和高度和给这个纹理的值是空的
	glTexImage2D(GL_TEXTURE_2D, 0, aTextureParam.InternalFormat, aTextureParam.width, aTextureParam.height,
		0, aTextureParam.Format, aTextureParam.DataType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glBindTexture(GL_TEXTURE_2D, 0);	
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + aAttachment, GL_TEXTURE_2D, color_buffer, 0);
	return color_buffer;
}

//创建一个带颜色缓冲附件的帧缓冲
unsigned CeTextureFormatter::CreateColorFramebuffer(ColorFramebufferParam& aParam)
{
	aParam.color_fbo = CreateFramebuffer();
	BindFramebuffer(aParam.color_fbo);
	glGenTextures(1, &aParam.color_buffer);
	glBindTexture(GL_TEXTURE_2D, aParam.color_buffer);
	//注意当前视口的宽度和高度和给这个纹理的值是空的
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aParam.width, aParam.height,
		0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aParam.color_buffer, 0);

	if (!CheckFramebuffer(aParam.color_fbo)) return 0;
	UnbindFrameBuffer();
	return aParam.color_fbo;
}


//这个函数可以保留，创建多重采样纹理缓冲
unsigned CeTextureFormatter::CreateMSColorbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum, int aAttachment)
{
	unsigned ms_colorbuffer1;

	glGenTextures(1, &ms_colorbuffer1);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ms_colorbuffer1);
	//注意当前视口的宽度和高度和给这个纹理的值是空的 4是采样点数量
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aSampleNum, GL_RGB16, aWidth, aHeight, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	//绑定纹理附件到帧缓冲，将它附加到GL_COLOR_ATTACHMENT0,最后的0是多级渐远纹理的级别，这里为0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + aAttachment, GL_TEXTURE_2D_MULTISAMPLE, ms_colorbuffer1, 0);
	return ms_colorbuffer1;
}

//颜色精度RGB16，创建一个带多重采样颜色缓冲的帧缓冲
unsigned CeTextureFormatter::CreateMSFramebuffer(MSFramebufferParam& aParam)
{
	//先创建帧缓冲
	aParam.ms_fbo = CreateFramebuffer();
	BindFramebuffer(aParam.ms_fbo);

	//创建一个深度缓冲附件
	aParam.ms_depth_attachment = CreateMSDepthRenderbuffer( aParam.width, aParam.height, aParam.sample);
	//创建多重采样颜色附件
	aParam.ms_colorbuffer1 = CreateMSColorbuffer(aParam.width, aParam.height, aParam.sample,0);

	aParam.ms_colorbuffer2 = CreateMSColorbuffer(aParam.width, aParam.height, aParam.sample,1);
	
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (!CheckFramebuffer(aParam.ms_fbo))return 0;	
	BindFramebuffer(0);
	return aParam.ms_fbo;
}


/*================================================================================================================*/



TextureFrameBufferHd CeTextureFormatter::CreateDepthTextureBuffer(TextureParam aTextureParam)
{
	TextureFrameBufferHd hd;
	 
	hd.FrambufferID = CreateFramebuffer();

	glGenTextures(1, &hd.AttachmenID);
	glBindTexture(GL_TEXTURE_2D, hd.AttachmenID);
	//注意当前视口的宽度和高度和format = depth component, type = float
	glTexImage2D(GL_TEXTURE_2D, 0, aTextureParam.Format,
		aTextureParam.width, aTextureParam.height, 0,
		aTextureParam.Format, aTextureParam.DataType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, hd.FrambufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, hd.AttachmenID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	return hd;
}



/*===========================================================================================================*/



TextureFrameBufferHd CeTextureFormatter::CreateRenderBuffer(RenderBufferParam aRenderBufferParam)
{
	TextureFrameBufferHd hd;
	 
	hd.FrambufferID = CreateFramebuffer();

	//创建渲染缓冲对象，GL_DEPTH24_STENCIL8是深度值和模板值的精度，4 是采样点数量
	glGenRenderbuffers(1, &hd.AttachmenID);
	glBindRenderbuffer(GL_RENDERBUFFER, hd.AttachmenID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, aRenderBufferParam.width, aRenderBufferParam.height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);//分配完内存之后可以解绑该缓冲对象

	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, hd.AttachmenID);

	//检查帧缓冲是否是完整的，否则打印错误信息
	if (!CheckFramebuffer(hd.FrambufferID)) return hd;

	//解绑配置好的帧缓冲，即回到默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	 
	return hd;
}

TextureFrameBufferHd CeTextureFormatter::CreateMSRenderBuffer(RenderBufferParam aRenderBufferParam, unsigned aSampleNum)
{
	TextureFrameBufferHd hd;
	glGenRenderbuffers(1, &hd.AttachmenID);
	//创建渲染缓冲对象，GL_DEPTH24_STENCIL8是深度值和模板值的精度
	glBindRenderbuffer(GL_RENDERBUFFER, hd.AttachmenID);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); // Allocate storage for render buffer object
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_RGB, aRenderBufferParam.width, aRenderBufferParam.height);

	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); // Attach MS render buffer object to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, hd.AttachmenID);
	//检查帧缓冲是否是完整的，否则打印错误信息
	if (CheckFramebuffer(hd.FrambufferID)) return hd;  
	return hd;
}

unsigned CeTextureFormatter::CreateMSDepthRenderbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum)
{
	unsigned ms_depth_buffer;
	//创建并附加一个深度渲染缓冲附件	
	glGenRenderbuffers(1, &ms_depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, ms_depth_buffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight);//这个是普通深度缓冲
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_DEPTH_COMPONENT, aWidth, aHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ms_depth_buffer);
	return ms_depth_buffer;
}

void CeTextureFormatter::CreateFloatFrameBuffer(TextureParam aParam, unsigned& aFBOHD, unsigned aTBONum,
	vector<unsigned>& aTBOs)
{	 
	/*
	参数：纹理目标 多级渐远级别，此纹理数据位数，宽度，高度，边界（废弃参数），输入数据通道，数据类型，输入数据
	默认的帧缓冲默认一个颜色分量只占用8位(bits)。当使用一个使用32位每颜色分量的浮点帧缓冲时(使用GL_RGB32F
	或者GL_RGBA32F)，我们需要四倍的内存来存储这些颜色。所以除非你需要一个非常高的精确度，32位不是必须的，
	使用GLRGB16F就足够了。
	*/
	aFBOHD = CreateFramebuffer();
	for (int i = 0; i < aTBONum; i++)
	{
		unsigned tbo = CreateTextureBuffer(aParam);
		aTBOs.push_back(tbo); 
	}
}

unsigned CeTextureFormatter::CreateDepthRenderBuffer(unsigned aFBO, unsigned aWidth, unsigned aHeight)
{
	unsigned int depth_rob;	
	glGenRenderbuffers(1, &depth_rob);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_rob);
	// 同时为深度和模板缓冲使用一个渲染缓冲对象
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, aWidth, aHeight); 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rob); // 附加到帧缓冲		
	return depth_rob;
}

void CeTextureFormatter::CreateHRDFramebuffer(HDRFramebufferParam & aParam)
{	
	glGenFramebuffers(1, &aParam.hdr_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, aParam.hdr_fbo);
	// create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
	//unsigned int colorBuffers[2];
	glGenTextures(2, aParam.hdr_colorbuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, aParam.hdr_colorbuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, aParam.width, aParam.height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, aParam.hdr_colorbuffer[i], 0);
	}

	//创建并附加一个深度渲染缓冲附件	
	glGenRenderbuffers(1, &aParam.hdr_depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, aParam.hdr_depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aParam.width, aParam.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, aParam.hdr_depthbuffer);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		DebugLogManager::DebugOutput("CeTextureFormatter", EDebugTag::EDT_ERROR, { "Framebuffer not complete." });
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	// 用于产生模糊效果的跳跃帧缓冲	
	glGenFramebuffers(2, aParam.pingpong_fbo);
	glGenTextures(2, aParam.pingpong_colorbuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, aParam.pingpong_fbo[i]);
		glBindTexture(GL_TEXTURE_2D, aParam.pingpong_colorbuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, aParam.width, aParam.height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aParam.pingpong_colorbuffer[i], 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		// 检查帧缓冲是否完整
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			DebugLogManager::DebugOutput("CeTextureFormatter", EDebugTag::EDT_ERROR, {"Framebuffer not complete."});
			return;
		}			
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void CeTextureFormatter::CreateFrambufferWithDepthTexture(FBOParam& aParam, unsigned& aTexID)
{	 
	glGenFramebuffers(1, &aParam.fbo);
 
	glGenTextures(1, &aTexID);
	glBindTexture(GL_TEXTURE_2D, aTexID);
	//注意纹理内部格式是GL_Depth_Component
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT,
		aParam.width, aParam.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//临近过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//超过光的投影矩阵规定的视锥的范围时，物体表面都显示在阴影中。
	//出现这个状况是因为超出光的视锥的投影坐标比1.0大，这样采样的深度纹理就会超出他默认的0到1的范围。
	//根据纹理环绕方式（gl_repeat），我们将会得到不正确的深度结果，它不是基于真实的来自光源的深度值。
	//让所有超出深度贴图的坐标的深度范围是1.0，这样超出的坐标将永远不在阴影之中。
	//可以储存一个边框颜色，然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER,这样所有超出纹理坐标
	//范围的采样结果都为1.0f即不在阴影中
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//不能重复环绕，因为
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLfloat borderColor[] = { 1.0, 1.0, 0.0, 1.0 };//好像没用
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, aParam.fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, aTexID, 0);
	//这个帧缓冲用于渲染深度贴图，所以禁止颜色缓冲到写入和读取
	//不包含颜色缓冲的帧缓冲是不完整的，因此需要显示告诉OpenGL不需要颜色缓冲
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void CeTextureFormatter::CreatePingpongFramebuffers(PingPongFramebufferParam & aParam)
{



}



/*===========================================================================================================*/



void CeTextureFormatter::DeleteTexture2D(unsigned aTexHD)
{
	glDeleteTextures(1,&aTexHD);
}
void CeTextureFormatter::DeleteTexture2D(const vector<unsigned>& aTexHDs)
{
	for (auto val : aTexHDs)
	{
		glDeleteTextures(1,&val);
	}
}
void CeTextureFormatter::ClearDepthbuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}
void CeTextureFormatter::ClearColorbffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
void CeTextureFormatter::ClearStencilbuffer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}
void CeTextureFormatter::ClearColorAndDepthbuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void CeTextureFormatter::ClearAllbuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
unsigned CeTextureFormatter::BindTexture2D(unsigned aShaderHD, unsigned aTexHD,  unsigned aTexUnit, unsigned aTexTarget)
{
	if (aTexUnit > 15)return 0;		

	glActiveTexture(GL_TEXTURE0 + aTexUnit);	
	glBindTexture(aTexTarget, aTexHD);	
	return aTexUnit + 1;

}

void CeTextureFormatter::BindTexture2D(unsigned aShaderIDs, const vector<unsigned>& aTexIDs,  unsigned aTexUnitOffset, unsigned aTexTarget)
{
	for (int i = 0; i < aTexIDs.size() && i < i < 15 - aTexUnitOffset; i++)
	{		
		glActiveTexture(GL_TEXTURE0 + i + aTexUnitOffset);			
		glBindTexture(aTexTarget, aTexIDs[i]);			
	}
}

void CeTextureFormatter::BindTexture2D(unsigned aTexHD, int aTexUnit)
{
	glActiveTexture(GL_TEXTURE0 + aTexUnit);
	glBindTexture(GL_TEXTURE_2D, aTexHD);
}


void CeTextureFormatter::BindTextureCube(unsigned aTexHD)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, aTexHD);
}

int CeTextureFormatter::BindAttachmentToFrameBuffer(unsigned aFrameBufferHD, unsigned aAttachmentHD, unsigned aAtttchmentTag)
{ 
	glBindFramebuffer(GL_FRAMEBUFFER,aFrameBufferHD);
	//绑定纹理附件到帧缓冲，将它附加到GL_COLOR_ATTACHMENT0,最后的0是多级渐远纹理的级别，这里为0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + aAtttchmentTag, GL_TEXTURE_2D, aAttachmentHD, 0);
	//检查帧缓冲是否是完整的，否则打印错误信息
	if(!CheckFramebuffer(aFrameBufferHD)) return 0;
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	return 1;
}

void CeTextureFormatter::BindFramebuffer(unsigned aFrameBufferHD)
{
	glBindFramebuffer(GL_FRAMEBUFFER, aFrameBufferHD);
}

void CeTextureFormatter::BindFramebuffer(unsigned aFBO, unsigned aWidth, unsigned aHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, aFBO);
	glViewport(0,0, aWidth, aHeight);
}

void CeTextureFormatter::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CeTextureFormatter::UnbindTexture2D()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool CeTextureFormatter::CheckFramebuffer(unsigned aFBO)
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		//删除失败帧缓冲
		glDeleteFramebuffers(1, &aFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		DebugLogManager::DebugOutput("CeTextureFormatter", EDebugTag::EDT_ERROR, { "Framebuffer not complete." });
		//如果帧缓冲创建失败，返回默认的帧缓冲对象
		return false;
	}	 
	return true;
}

void CeTextureFormatter::ExchangeColorbuffer(unsigned aFBO, unsigned aTargetFBO, unsigned aWidth, unsigned aHeight, unsigned aAttachmentNum)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, aFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, aTargetFBO);
	
	for (int i = 0; i < 15 && i < aAttachmentNum; i++)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
		glBlitFramebuffer(0, 0, aWidth, aHeight, 0, 0, aWidth, aHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}	
	/*glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, aWidth, aHeight, 0, 0, aWidth, aHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, aWidth, aHeight, 0, 0, aWidth, aHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);*/
	//glCreateFramebuffers();
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
