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
 * @brief 该文件封装库启动、一些绘制函数、帧缓冲处理、离屏渲染、纹理处理
 * 将编译器和网格处理独立出去
 * @note 这个文件是很老的文件了，是作者初学C++以及Opengl的时候写的，已经废除
 */
namespace CircleEngine
{

//宏定义
#define ROW(a) #a

//全局变量定义
const extern char* line_vretext_shader;
const extern char* rectangle_vertex_shader;
const extern char* line_fragment_shader;
const extern char* rectangle_fragment_shader;

////事件定义
//
////着色器编译错误事件  直接写一个引擎事件管理，就是日志系统
//
//Delegate<int> caonia;



/**
 * @brief 初始化GLEW库
 */
bool InitGlew();


void DrawWithIndices(unsigned aVAO, int aIndicesSize);

//shader 编译 
unsigned ShaderCompile(const char* vShader, const char* fShadher, const char* gShader); 

//创建一个VAO
unsigned CreateVAO();

//创建一个填充了顶点的VBO 静态的
unsigned CreateVBO(
	unsigned VAO_,
	float* vertices,
	unsigned total_data_size_of_float,//数据总共几个float
	unsigned vector_demension_of_float,//一个位置向量（纹理坐标，法向量）的维度
	unsigned Attrib_total_demension_of_float,//一个顶点属性的总维度（该顶点属性包括：位置向量纹理坐标，法向量）
	unsigned offset_ = 0,//数据每隔几个
	unsigned layout_ = 0);




 /**
* @brief 分批顶点属性
* @param[in] VAO 顶点数组对象句柄
* @param[in] position_info 顶点数据及其格式信息。按照顺序：数组指针，数组大小，单个数据维度，步幅 
* @param[in] texture_info 纹理坐标数组及其信息。
*/
unsigned CreateVBO(unsigned aVAO, tuple<float*, unsigned, unsigned, unsigned> aVertexInfo, tuple<float*, unsigned, unsigned, unsigned> aTexCoordInfo);



//分批顶点属性，带法向量
unsigned CreateVBO(unsigned aVAO, tuple<float*, unsigned, unsigned, unsigned>aVertexInfo, tuple<float*, unsigned, unsigned, unsigned>aTexCoordInfo, tuple<float*, unsigned, unsigned, unsigned>aNormalInfo);

//解析模型数据，因为结构体内存布局是连续的，所以传入一个结构体指针
//为啥又不用分批顶点属性了？因为Assimp库输出的顶点信息是交错分布的，
//当然也可以转换为连续的，后续改一下

/**
* @brief 读取模型后初始化模型的网格
* @param aVAO 顶点数组对象
* @param aMeshInfo 网格（结构体）信息。包含顶点、索引等信息。
*/
//unsigned CreateVBO(unsigned aVAO,const Mesh& aMeshInfo);

/**
 * @brief 创建一个空的分配了指定内存空间的VBO
 * @param total_data_size_of_float 数据总共几个float(顶点数组总大小)
 * @param data_sets_menber_num 一组数据由几个成员组成
 * @param data_size_of_float 一组数据总共几个float(几维向量)
 * @param offset 数据分布间隔
 * @param layout 顶点信息层次 layout belong to [0,16]
 * @return VBO　顶点数组对象ID
 * @note 仅适用于float数组，仅适用于动态顶点存储
 */
unsigned CreateVBOEmpty(
	unsigned VAO,
	unsigned total_data_size_of_float,//数据总共几个float
	unsigned data_sets_menber_num,//一组数据分成几个
	unsigned data_size_of_float,//一组数据总共几个float
	unsigned offset = 0,//数据每隔几个
	unsigned layout = 0);  

/**
* @brief 顶点索引信息
* @property ElementPtr
* @property ElementSize
* @property RenderMode 默认值为GL_STATIC_DRAW
*/
struct ElemenBuffertInfo
{
	//索引数组的指针
	unsigned* ElementPtr = 0;	
	//索引数组大小
	int ElementSize = 0;
	//顶点绘制模式，即是否会改变顶点的值。 Dynami 或者 Static
	unsigned RenderMode = 0x88E0;//GL_STREAM_DRAW 0x88E0
};

//创建一个 EBO
unsigned CreateEBO(unsigned aVAO, unsigned * aIndices, unsigned aDataSizeOfFloat, unsigned aLayout);
unsigned CreateEBO(unsigned aVAO, const ElemenBuffertInfo& aInfo);
//重写动态VBO的值
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
//* @brief 纹理过滤方式
//* GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。
//* 当设置为GL_NEAREST的时候，OpenGL会选择中心点最接近纹理坐标的那个像素。下图中你可以看到
//* 四个像素，加号代表纹理坐标。左上角那个纹理像素的中心距离纹理坐标最近，所以它会被选择为样
//* 本颜色：
//* GL_LINEAR（也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一
//* 个插值，近似出这些纹理像素之间的颜色。一个纹理像素的中心距离纹理坐标越近，那么这个纹理像素
//* 的颜色对最终的样本颜色的贡献越大。
//
//* 多级渐远纹理
//* 过滤方式						描述
//* GL_NEAREST_MIPMAP_NEAREST	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
//* GL_LINEAR_MIPMAP_NEAREST	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
//* GL_NEAREST_MIPMAP_LINEAR	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
//* GL_LINEAR_MIPMAP_LINEAR	在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
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
//* 环绕方式				描述
//* GL_REPEAT				对纹理的默认行为。重复纹理图像。
//* GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
//* GL_CLAMP_TO_EDGE		纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
//* GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
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
//* @brief 创建一个2D纹理，返回该纹理的Handle
//* @param aData	纹理数据
//* @param aWidth 纹理宽度
//* @param aHeight 纹理高度
//* @param aChannels 纹理颜色通道数量
//* @return 纹理信息
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
////创建一个帧缓冲并返回它
//unsigned CreateFramebuffer();
//unsigned CreateFramebuffer(unsigned aTextureHandle);
//
////创建一个纹理缓冲,返回附加了这个纹理附件的帧缓冲对象，实际上就是创建了一个空纹理
////注意一个帧缓冲只需要一个纹理或者一个渲染缓冲对象！
//TFrameBufferID CreateTextureBuffer(TTextureParam aTextureParam);
//
////创建一个用于渲染深度贴图的纹理
//TFrameBufferID CreateDepthTextureBuffer(TTextureParam aTextureParam);
//
//TFrameBufferID CreateMSColorbuffer(TTextureParam aTextureParam, unsigned aSampleNum); 
//
////创建一个渲染缓冲对象,返回附加了这个渲染缓冲附件的帧缓冲对象
//TFrameBufferID CreateRenderBuffer(TRenderBufferParam aRenderBufferParam);
//TFrameBufferID CreateMSRenderBuffer(TRenderBufferParam aRenderBufferParam, unsigned aSampleNum);
//
////创建一个帧缓冲对象
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