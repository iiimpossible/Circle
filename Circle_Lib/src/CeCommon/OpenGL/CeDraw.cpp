#include "CeDraw.h"
//#define GLEW_STATIC
#include <GL/glew.h>
//#include <glad/glad.h>
#include <CeEngine/Data/InlineData.h>
int CeGraphDrawing::InitDrawAPI()
{
	glewExperimental = true;	
	int tag = glewInit();
	if (!tag == GLEW_OK)
	{
		
		return 1;
	}
	
	if (!tag)
		return 1;	
	return 1;
}

void CeGraphDrawing::DrawGraph(unsigned aVAO, int aVerticesNum, EPel aPel)
{
	glBindVertexArray(aVAO);///<绑定顶点数组对象
	glDrawArrays(aPel, 0, aVerticesNum);///<这个顶点数目是会改变的，只不过目前只渲染方形
	glBindVertexArray(0);///<解绑
}

void CeGraphDrawing::DrawGraphWithIndice(unsigned aVAO, int aIndicesNum, EPel aPel)
{
	glBindVertexArray(aVAO);
	glDrawElements(aPel, aIndicesNum, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CeGraphDrawing::DrawFramebuffer(unsigned aBufferNum, unsigned aAttachments[])
{
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(aBufferNum, aAttachments);	 
}

void CeGraphDrawing::DrawFramebuffer(unsigned aBuffer)
{
	glDrawBuffer(aBuffer);
}

void CeGraphDrawing::ClearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CeGraphDrawing::DrawFullScreen()
{
	static FullSreenQuad quad;
	glBindVertexArray(quad.GetVAO());
	glDrawArrays(GL_TRIANGLES,0,quad.GetVertexiceNum());
	glBindVertexArray(0);

}

void CeGraphDrawing::DrawSkybox(unsigned aShaderHD, unsigned aVAO, unsigned aTextureHD, int aVerticesNum)
{
	glUseProgram(aShaderHD);
	glBindTexture(GL_TEXTURE_CUBE_MAP,aTextureHD);
	glBindVertexArray(aVAO);
	glDrawElements(GL_TRIANGLES, aVerticesNum, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);	

}
