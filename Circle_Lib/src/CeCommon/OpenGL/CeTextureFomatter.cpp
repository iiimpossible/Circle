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
	{	//�ڶ��������Ƕ༶��Զ������0Ϊ�������𡣳ߴ��ߵ�0����ʷ�������⣬����0ֵ
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aWidth, aHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, aData);
	}
	else if (aChannels == 3)	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aWidth, aHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, aData);
	else	//�����ǻҶ�ͼ��ͨ������Ϊ1
		return {};
	
	glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//�������������2D�������ͣ�s����t������,�����3D������һ��r,��x,y,z�ȼۡ��ظ�����
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
	//��ȫ�Լ��
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
	//ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ͸���������ֵ�ǿյ�
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

//����һ������ɫ���帽����֡����
unsigned CeTextureFormatter::CreateColorFramebuffer(ColorFramebufferParam& aParam)
{
	aParam.color_fbo = CreateFramebuffer();
	BindFramebuffer(aParam.color_fbo);
	glGenTextures(1, &aParam.color_buffer);
	glBindTexture(GL_TEXTURE_2D, aParam.color_buffer);
	//ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ͸���������ֵ�ǿյ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, aParam.width, aParam.height,
		0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aParam.color_buffer, 0);

	if (!CheckFramebuffer(aParam.color_fbo)) return 0;
	UnbindFrameBuffer();
	return aParam.color_fbo;
}


//����������Ա������������ز���������
unsigned CeTextureFormatter::CreateMSColorbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum, int aAttachment)
{
	unsigned ms_colorbuffer1;

	glGenTextures(1, &ms_colorbuffer1);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ms_colorbuffer1);
	//ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ͸���������ֵ�ǿյ� 4�ǲ���������
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aSampleNum, GL_RGB16, aWidth, aHeight, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	//����������֡���壬�������ӵ�GL_COLOR_ATTACHMENT0,����0�Ƕ༶��Զ����ļ�������Ϊ0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + aAttachment, GL_TEXTURE_2D_MULTISAMPLE, ms_colorbuffer1, 0);
	return ms_colorbuffer1;
}

//��ɫ����RGB16������һ�������ز�����ɫ�����֡����
unsigned CeTextureFormatter::CreateMSFramebuffer(MSFramebufferParam& aParam)
{
	//�ȴ���֡����
	aParam.ms_fbo = CreateFramebuffer();
	BindFramebuffer(aParam.ms_fbo);

	//����һ����Ȼ��帽��
	aParam.ms_depth_attachment = CreateMSDepthRenderbuffer( aParam.width, aParam.height, aParam.sample);
	//�������ز�����ɫ����
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
	//ע�⵱ǰ�ӿڵĿ�Ⱥ͸߶Ⱥ�format = depth component, type = float
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

	//������Ⱦ�������GL_DEPTH24_STENCIL8�����ֵ��ģ��ֵ�ľ��ȣ�4 �ǲ���������
	glGenRenderbuffers(1, &hd.AttachmenID);
	glBindRenderbuffer(GL_RENDERBUFFER, hd.AttachmenID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, aRenderBufferParam.width, aRenderBufferParam.height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);//�������ڴ�֮����Խ��û������

	//����Ⱦ������󸽼ӵ�֡�������Ⱥ�ģ�帽����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, hd.AttachmenID);

	//���֡�����Ƿ��������ģ������ӡ������Ϣ
	if (!CheckFramebuffer(hd.FrambufferID)) return hd;

	//������úõ�֡���壬���ص�Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	 
	return hd;
}

TextureFrameBufferHd CeTextureFormatter::CreateMSRenderBuffer(RenderBufferParam aRenderBufferParam, unsigned aSampleNum)
{
	TextureFrameBufferHd hd;
	glGenRenderbuffers(1, &hd.AttachmenID);
	//������Ⱦ�������GL_DEPTH24_STENCIL8�����ֵ��ģ��ֵ�ľ���
	glBindRenderbuffer(GL_RENDERBUFFER, hd.AttachmenID);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); // Allocate storage for render buffer object
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_RGB, aRenderBufferParam.width, aRenderBufferParam.height);

	//����Ⱦ������󸽼ӵ�֡�������Ⱥ�ģ�帽����
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); // Attach MS render buffer object to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, hd.AttachmenID);
	//���֡�����Ƿ��������ģ������ӡ������Ϣ
	if (CheckFramebuffer(hd.FrambufferID)) return hd;  
	return hd;
}

unsigned CeTextureFormatter::CreateMSDepthRenderbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum)
{
	unsigned ms_depth_buffer;
	//����������һ�������Ⱦ���帽��	
	glGenRenderbuffers(1, &ms_depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, ms_depth_buffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight);//�������ͨ��Ȼ���
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, aSampleNum, GL_DEPTH_COMPONENT, aWidth, aHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ms_depth_buffer);
	return ms_depth_buffer;
}

void CeTextureFormatter::CreateFloatFrameBuffer(TextureParam aParam, unsigned& aFBOHD, unsigned aTBONum,
	vector<unsigned>& aTBOs)
{	 
	/*
	����������Ŀ�� �༶��Զ���𣬴���������λ������ȣ��߶ȣ��߽磨��������������������ͨ�����������ͣ���������
	Ĭ�ϵ�֡����Ĭ��һ����ɫ����ֻռ��8λ(bits)����ʹ��һ��ʹ��32λÿ��ɫ�����ĸ���֡����ʱ(ʹ��GL_RGB32F
	����GL_RGBA32F)��������Ҫ�ı����ڴ����洢��Щ��ɫ�����Գ�������Ҫһ���ǳ��ߵľ�ȷ�ȣ�32λ���Ǳ���ģ�
	ʹ��GLRGB16F���㹻�ˡ�
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
	// ͬʱΪ��Ⱥ�ģ�建��ʹ��һ����Ⱦ�������
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, aWidth, aHeight); 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rob); // ���ӵ�֡����		
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

	//����������һ�������Ⱦ���帽��	
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

	
	// ���ڲ���ģ��Ч������Ծ֡����	
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
		// ���֡�����Ƿ�����
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
	//ע�������ڲ���ʽ��GL_Depth_Component
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT,
		aParam.width, aParam.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//�ٽ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//�������ͶӰ����涨����׶�ķ�Χʱ��������涼��ʾ����Ӱ�С�
	//�������״������Ϊ���������׶��ͶӰ�����1.0�������������������ͻᳬ����Ĭ�ϵ�0��1�ķ�Χ��
	//���������Ʒ�ʽ��gl_repeat�������ǽ���õ�����ȷ����Ƚ���������ǻ�����ʵ�����Թ�Դ�����ֵ��
	//�����г��������ͼ���������ȷ�Χ��1.0���������������꽫��Զ������Ӱ֮�С�
	//���Դ���һ���߿���ɫ��Ȼ��������ͼ��������ѡ������ΪGL_CLAMP_TO_BORDER,�������г�����������
	//��Χ�Ĳ��������Ϊ1.0f��������Ӱ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//�����ظ����ƣ���Ϊ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLfloat borderColor[] = { 1.0, 1.0, 0.0, 1.0 };//����û��
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, aParam.fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, aTexID, 0);
	//���֡����������Ⱦ�����ͼ�����Խ�ֹ��ɫ���嵽д��Ͷ�ȡ
	//��������ɫ�����֡�����ǲ������ģ������Ҫ��ʾ����OpenGL����Ҫ��ɫ����
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
	//����������֡���壬�������ӵ�GL_COLOR_ATTACHMENT0,����0�Ƕ༶��Զ����ļ�������Ϊ0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + aAtttchmentTag, GL_TEXTURE_2D, aAttachmentHD, 0);
	//���֡�����Ƿ��������ģ������ӡ������Ϣ
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
		//ɾ��ʧ��֡����
		glDeleteFramebuffers(1, &aFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		DebugLogManager::DebugOutput("CeTextureFormatter", EDebugTag::EDT_ERROR, { "Framebuffer not complete." });
		//���֡���崴��ʧ�ܣ�����Ĭ�ϵ�֡�������
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
