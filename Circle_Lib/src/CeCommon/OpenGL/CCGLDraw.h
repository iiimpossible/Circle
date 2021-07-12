#pragma once
#include <iostream>
#include <tuple>

#include "../Tool/Delegate.h"
#include "../Math/Math.h"
#include "../Common.h"
#include <Celib_x64/src/CeCommon/Tool/CeStructs.h>
#include <Celib_x64/src/CeEngine/Render/Mesh.h>
using  std::string ;
using std::tuple;




/**
 * @brief ���ļ���װ��������һЩ���ƺ�����֡���崦��������Ⱦ��������
 * ���������������������ȥ
 * @note ����ļ��Ǻ��ϵ��ļ��ˣ������߳�ѧC++�Լ�Opengl��ʱ��д�ģ��Ѿ��ϳ�
 */
namespace CircleEngine
{

//�궨��
#define ROW(a) #a

//ȫ�ֱ�������
const extern char* line_vretext_shader;
const extern char* rectangle_vertex_shader;
const extern char* line_fragment_shader;
const extern char* rectangle_fragment_shader;

////�¼�����
//
////��ɫ����������¼�  ֱ��дһ�������¼�����������־ϵͳ
//
//Delegate<int> caonia;



/**
 * @brief ��ʼ��GLEW��
 */
bool InitGlew();


void DrawWithIndices(unsigned aVAO, int aIndicesSize);

//shader ���� 
unsigned ShaderCompile(const char* vShader, const char* fShadher, const char* gShader); 

//����һ��VAO
unsigned CreateVAO();

//����һ������˶����VBO ��̬��
unsigned CreateVBO(
	unsigned VAO_,
	float* vertices,
	unsigned total_data_size_of_float,//�����ܹ�����float
	unsigned vector_demension_of_float,//һ��λ���������������꣬����������ά��
	unsigned Attrib_total_demension_of_float,//һ���������Ե���ά�ȣ��ö������԰�����λ�������������꣬��������
	unsigned offset_ = 0,//����ÿ������
	unsigned layout_ = 0);




 /**
* @brief ������������
* @param[in] VAO �������������
* @param[in] position_info �������ݼ����ʽ��Ϣ������˳������ָ�룬�����С����������ά�ȣ����� 
* @param[in] texture_info �����������鼰����Ϣ��
*/
unsigned CreateVBO(unsigned aVAO, tuple<float*, unsigned, unsigned, unsigned> aVertexInfo, tuple<float*, unsigned, unsigned, unsigned> aTexCoordInfo);



//�����������ԣ���������
unsigned CreateVBO(unsigned aVAO, tuple<float*, unsigned, unsigned, unsigned>aVertexInfo, tuple<float*, unsigned, unsigned, unsigned>aTexCoordInfo, tuple<float*, unsigned, unsigned, unsigned>aNormalInfo);

//����ģ�����ݣ���Ϊ�ṹ���ڴ沼���������ģ����Դ���һ���ṹ��ָ��
//Ϊɶ�ֲ��÷������������ˣ���ΪAssimp������Ķ�����Ϣ�ǽ���ֲ��ģ�
//��ȻҲ����ת��Ϊ�����ģ�������һ��

/**
* @brief ��ȡģ�ͺ��ʼ��ģ�͵�����
* @param aVAO �����������
* @param aMeshInfo ���񣨽ṹ�壩��Ϣ���������㡢��������Ϣ��
*/
//unsigned CreateVBO(unsigned aVAO,const Mesh& aMeshInfo);

/**
 * @brief ����һ���յķ�����ָ���ڴ�ռ��VBO
 * @param total_data_size_of_float �����ܹ�����float(���������ܴ�С)
 * @param data_sets_menber_num һ�������ɼ�����Ա���
 * @param data_size_of_float һ�������ܹ�����float(��ά����)
 * @param offset ���ݷֲ����
 * @param layout ������Ϣ��� layout belong to [0,16]
 * @return VBO�������������ID
 * @note ��������float���飬�������ڶ�̬����洢
 */
unsigned CreateVBOEmpty(
	unsigned VAO,
	unsigned total_data_size_of_float,//�����ܹ�����float
	unsigned data_sets_menber_num,//һ�����ݷֳɼ���
	unsigned data_size_of_float,//һ�������ܹ�����float
	unsigned offset = 0,//����ÿ������
	unsigned layout = 0);  

/**
* @brief ����������Ϣ
* @property ElementPtr
* @property ElementSize
* @property RenderMode Ĭ��ֵΪGL_STATIC_DRAW
*/
struct ElemenBuffertInfo
{
	//���������ָ��
	unsigned* ElementPtr = 0;	
	//���������С
	int ElementSize = 0;
	//�������ģʽ�����Ƿ��ı䶥���ֵ�� Dynami ���� Static
	unsigned RenderMode = 0x88E0;//GL_STREAM_DRAW 0x88E0
};

//����һ�� EBO
unsigned CreateEBO(unsigned aVAO, unsigned * aIndices, unsigned aDataSizeOfFloat, unsigned aLayout);
unsigned CreateEBO(unsigned aVAO, const ElemenBuffertInfo& aInfo);
//��д��̬VBO��ֵ
void ReWriteVBO(unsigned aVBO, float * aNewVertices, unsigned aDataSizeOfFloat, unsigned aStartIndex = 0);







void UseProgram(const unsigned int aShaderHandle);

void BindTexture(unsigned aShaderHandle, unsigned aTextureHandle, const char* aTextureName, unsigned aTextureUnit = 0, unsigned aTextureTarget = 0x0DE1);// GL_TEXTURE_2D 0x0DE1

//void SetBool(const unsigned int aShaderHandle, const string& aName, bool aValue);
//
//void SetInt(const unsigned int aShaderHandle, const string& aName, int aValue);
//void SetInt(const unsigned int aShaderHandle, const string& aName, unsigned aElmentNum, int* aValue);
//
//void SetFloat(const unsigned int aShaderHandle, const string& aName, float aValue);
//void SetFloat(const unsigned int aShaderHandle, const string& aName,  unsigned aElmentNum,float* aValue);
//
//void SetVec2(const unsigned int aShaderHandle, const string & aName, const Vec2 & aValue);
//void SetVec2(const unsigned int aShaderHandle, const string & aName, unsigned aElementNum, const Vec2 & aValue);
//
////void SetVec2(const unsigned int shader_id, const string &aName, float x, float y);
//void SetVec3(const unsigned int aShaderHandle, const string& aName, const Vec3& aVctor);
////void setVec3(const unsigned int aShaderID, const string &aName, float x, float y, float z);
//void SetVec4(const unsigned int aShaderHandle, const string& aName, const Vec4& aVctor);
////void SetVec4(const unsigned int aShaderID, const string &aName, float x, float y, float z, float w);
//void SetMat2(const unsigned int aShaderHandle, const string& aName, const Mat2& aMat);
//void SetMat3(const unsigned int aShaderHandle, const string& aName, const Mat3& aMat);
//void SetMat4(const unsigned int aShaderHandle, const string& aName, const Mat4& aMat);


///**
//* @brief ������˷�ʽ
//* GL_NEAREST��Ҳ���ڽ����ˣ�Nearest Neighbor Filtering����OpenGLĬ�ϵ�������˷�ʽ��
//* ������ΪGL_NEAREST��ʱ��OpenGL��ѡ�����ĵ���ӽ�����������Ǹ����ء���ͼ������Կ���
//* �ĸ����أ��ӺŴ����������ꡣ���Ͻ��Ǹ��������ص����ľ�����������������������ᱻѡ��Ϊ��
//* ����ɫ��
//* GL_LINEAR��Ҳ�����Թ��ˣ�(Bi)linear Filtering����������������긽�����������أ������һ
//* ����ֵ�����Ƴ���Щ��������֮�����ɫ��һ���������ص����ľ�����������Խ������ô�����������
//* ����ɫ�����յ�������ɫ�Ĺ���Խ��
//
//* �༶��Զ����
//* ���˷�ʽ						����
//* GL_NEAREST_MIPMAP_NEAREST	ʹ�����ڽ��Ķ༶��Զ������ƥ�����ش�С����ʹ���ڽ���ֵ�����������
//* GL_LINEAR_MIPMAP_NEAREST	ʹ�����ڽ��Ķ༶��Զ�����𣬲�ʹ�����Բ�ֵ���в���
//* GL_NEAREST_MIPMAP_LINEAR	��������ƥ�����ش�С�Ķ༶��Զ����֮��������Բ�ֵ��ʹ���ڽ���ֵ���в���
//* GL_LINEAR_MIPMAP_LINEAR	�������ڽ��Ķ༶��Զ����֮��ʹ�����Բ�ֵ����ʹ�����Բ�ֵ���в���
//*/
//enum ETextureFilter
//{
//	ETF_Nearest = 0x2600,//GL_NEAREST 0x2600
//	ETF_Linear = 0x2601,// GL_LINEAR 0x2601
//	EM_NearestMipmapNearest = 0x2700,//GL_NEAREST_MIPMAP_NEAREST 0x2700
//	EM_LinearMipmapNearest = 0x2701,// GL_LINEAR_MIPMAP_NEAREST 0x2701
//	EM_NearestMipmapLinear = 0x2702,// GL_NEAREST_MIPMAP_LINEAR 0x2702
//	EM_LinearMipmapLinear = 0x2703//GL_LINEAR_MIPMAP_LINEAR 0x2703
//};
//
//
///**
//* @brief
//* ���Ʒ�ʽ				����
//* GL_REPEAT				�������Ĭ����Ϊ���ظ�����ͼ��
//* GL_MIRRORED_REPEAT	��GL_REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
//* GL_CLAMP_TO_EDGE		��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
//* GL_CLAMP_TO_BORDER	����������Ϊ�û�ָ���ı�Ե��ɫ��
//*/
//enum ETextureAround
//{
//	ETA_Repeat = 0x2901,//GL_REPEAT 0x2901
//	ETA_MirroredRepeat = 0x8370,// GL_MIRRORED_REPEAT 0x8370
//	ETA_ClampToEdge = 0x812F,//GL_CLAMP_TO_EDGE 0x812F
//	ETA_ClampToBorder = 0x812D// GL_CLAMP_TO_BORDER 0x812D
//};
// 
//
//struct Texture2DParam
//{
//	unsigned char* TextureData;
//	int Width;
//	int Height;
//	int Channel;
//	ETextureAround AroundS = ETextureAround::ETA_Repeat;
//	ETextureAround AroundT = ETextureAround::ETA_Repeat;
//	ETextureFilter FilterMIN = ETextureFilter::ETF_Nearest;
//	ETextureFilter FilterMGA = ETextureFilter::ETF_Nearest;
//};
//
///**
//* @brief ����һ��2D�������ظ������Handle
//* @param aData	��������
//* @param aWidth ������
//* @param aHeight ����߶�
//* @param aChannels ������ɫͨ������
//* @return ������Ϣ
//*/
//Texture2DInfo CreateTexture2D(unsigned char* aData, int aWidth, int aHeight, int aChannels, 
//	ETextureAround aTextureAroundS = ETextureAround::ETA_Repeat, 
//	ETextureAround aTextureAroundT = ETextureAround::ETA_Repeat,
//	ETextureFilter aTextureFilterMIN = ETextureFilter::ETF_Nearest,
//	ETextureFilter aTextureFilterMGA = ETextureFilter::ETF_Nearest);
//
//Texture2DInfo CreateTexture2D(const Texture2DParam& aParam);


//#pragma region OpenGL_FrameBuffer
//
//enum ETextureFormat
//{
//	//GL_RGB 0x1907
//	//GL_RGBA 0x1908
//	//GL_DEPTH_COMPONENT 0x1902
//	ETF_DepthComponent = 0x1902,
//	ETF_RGB = 0x1907,
//	ETF_RGBA = 0x1908
//};
//
//enum ETextureDataType
//{
//	/*GL_BYTE 0x1400
//	  GL_UNSIGNED_BYTE 0x1401
//	  GL_SHORT 0x1402
//	  GL_UNSIGNED_SHORT 0x1403
//	  GL_INT 0x1404
//	  GL_UNSIGNED_INT 0x1405
//	  GL_FLOAT 0x1406
//	  GL_2_BYTES 0x1407
//	  GL_3_BYTES 0x1408
//	  GL_4_BYTES 0x1409
//	  GL_DOUBLE 0x140A*/
//	ETD_Byte = 0x1400,
//	ETD_UnsignedByte = 0x1401,
//	ETD_Short = 0x1402,
//	ETD_UnsignedShort = 0x1403,
//	ETD_Int = 0x1404,
//	ETD_UnsignedInt = 0x1405,
//	ETD_Float = 0x1406,
//	ETD_2Bytes = 0x1407,
//	ETD_3Bytes = 0x1408,
//	ETD_4Bytes = 0x1409,
//	ETD_Double = 0x140A
//};
//struct TTextureParam
//{
//	unsigned Width;
//	unsigned Height; 
//	ETextureFormat Format = ETextureFormat::ETF_RGB;
//	ETextureDataType DataType = ETextureDataType::ETD_Float;
//};
//
//
//struct TRenderBufferParam
//{
//	unsigned FrameBufferID;
//	unsigned Width;
//	unsigned Height;
//
//	
//};
//
//struct TFrameBufferID
//{
//	unsigned FrambufferID;
//	unsigned AttachmenID;
//};
//
////����һ��֡���岢������
//unsigned CreateFramebuffer();
//unsigned CreateFramebuffer(unsigned aTextureHandle);
//
////����һ��������,���ظ����������������֡�������ʵ���Ͼ��Ǵ�����һ��������
////ע��һ��֡����ֻ��Ҫһ���������һ����Ⱦ�������
//TFrameBufferID CreateTextureBuffer(TTextureParam aTextureParam);
//
////����һ��������Ⱦ�����ͼ������
//TFrameBufferID CreateDepthTextureBuffer(TTextureParam aTextureParam);
//
//TFrameBufferID CreateMSColorbuffer(TTextureParam aTextureParam, unsigned aSampleNum); 
//
////����һ����Ⱦ�������,���ظ����������Ⱦ���帽����֡�������
//TFrameBufferID CreateRenderBuffer(TRenderBufferParam aRenderBufferParam);
//TFrameBufferID CreateMSRenderBuffer(TRenderBufferParam aRenderBufferParam, unsigned aSampleNum);
//
////����һ��֡�������
//
//unsigned CreateFramebuffer();
//unsigned CreateTextureBuffer(unsigned aWidth, unsigned aHeight);
//unsigned CreateMSColorbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum);
//unsigned CreateRenderBuffer(unsigned aWidth, unsigned aHeight);
//unsigned CreateMSRenderBuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum);
//
//void BindFrameBuffer();
//void BindTextureBuffer();
//void BindMSTextureBuffer();
//void BindRenderBuffer();
//void BindMSTextureBuffer();
//void CheckFrameBuffer();
//void UnBindFrameBuffer();

#pragma endregion



}