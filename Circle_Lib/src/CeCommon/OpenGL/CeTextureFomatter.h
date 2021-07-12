#pragma once
#include <string>
#include <vector>
#include <CeCommon/Common.h>
using std::string;
using std::vector;



//GL_RGB 0x1907
//GL_RGBA 0x1908
//GL_DEPTH_COMPONENT 0x1902
//#define GL_RGB4 0x804F
//#define GL_RGB5 0x8050
//#define GL_RGB8 0x8051
//#define GL_RGB10 0x8052
//#define GL_RGB12 0x8053
//#define GL_RGB16 0x8054
//#define GL_RGBA2 0x8055
//#define GL_RGBA4 0x8056
//#define GL_RGB5_A1 0x8057
//#define GL_RGBA8 0x8058
//#define GL_RGB10_A2 0x8059
//#define GL_RGBA12 0x805A
//#define GL_RGBA16 0x805B

//#define GL_ARB_texture_multisample 1
//
//#define GL_SAMPLE_POSITION 0x8E50
//#define GL_SAMPLE_MASK 0x8E51
//#define GL_SAMPLE_MASK_VALUE 0x8E52
//#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
//#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
//#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
//#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
//#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
//#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
//#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
//#define GL_TEXTURE_SAMPLES 0x9106
//#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
//#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
//#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
//#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
//#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
//#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
//#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
//#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
//#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
//#define GL_MAX_INTEGER_SAMPLES 0x9110


//#define GL_EXT_sparse_texture 1
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
enum ETextureFormat
{
	ETF_DepthComponent = 0x1902,

	//纹理通道及其颜色精度
	ETF_RGB = 0x1907,
	ETF_RGBA = 0x1908,
	ETF_RGB16 = 0x8054,
	ETF_RGBA16 = 0x805B,

	//纹理目标
	ETF_Texture2D = 0x0DE1,//普通2D纹理
	ETF_Texture3D = 0x806F,//3D纹理
	ETF_TextureCubeMap = 0x8513,//天空盒子
	ETF_Texture2DMultiSample = 0x9100,//多重采样2D纹理

};

enum ETextureDataType
{
	/*GL_BYTE 0x1400
	  GL_UNSIGNED_BYTE 0x1401
	  GL_SHORT 0x1402
	  GL_UNSIGNED_SHORT 0x1403
	  GL_INT 0x1404
	  GL_UNSIGNED_INT 0x1405
	  GL_FLOAT 0x1406
	  GL_2_BYTES 0x1407
	  GL_3_BYTES 0x1408
	  GL_4_BYTES 0x1409
	  GL_DOUBLE 0x140A*/
	ETD_Byte = 0x1400,
	ETD_UnsignedByte = 0x1401,
	ETD_Short = 0x1402,
	ETD_UnsignedShort = 0x1403,
	ETD_Int = 0x1404,
	ETD_UnsignedInt = 0x1405,
	ETD_Float = 0x1406,
	ETD_2Bytes = 0x1407,
	ETD_3Bytes = 0x1408,
	ETD_4Bytes = 0x1409,
	ETD_Double = 0x140A
};
struct TextureParam
{
	unsigned width = 800;
	unsigned height = 600;

	ETextureFormat InternalFormat = ETextureFormat::ETF_RGB16;
	ETextureFormat Format = ETextureFormat::ETF_RGB16;
	ETextureDataType DataType = ETextureDataType::ETD_Float;
};


struct RenderBufferParam
{
	unsigned FrameBufferID;
	unsigned width = 800;
	unsigned height = 600;
};

struct TextureFrameBufferHd
{
	unsigned FrambufferID;
	unsigned AttachmenID;
};



/**
* @brief 纹理过滤方式
* GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。
* 当设置为GL_NEAREST的时候，OpenGL会选择中心点最接近纹理坐标的那个像素。下图中你可以看到
* 四个像素，加号代表纹理坐标。左上角那个纹理像素的中心距离纹理坐标最近，所以它会被选择为样
* 本颜色：
* GL_LINEAR（也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一
* 个插值，近似出这些纹理像素之间的颜色。一个纹理像素的中心距离纹理坐标越近，那么这个纹理像素
* 的颜色对最终的样本颜色的贡献越大。

* 多级渐远纹理
* 过滤方式						描述
* GL_NEAREST_MIPMAP_NEAREST	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
* GL_LINEAR_MIPMAP_NEAREST	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
* GL_NEAREST_MIPMAP_LINEAR	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
* GL_LINEAR_MIPMAP_LINEAR	在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
*/
enum ETexFilter
{
	ETF_Nearest = 0x2600,//GL_NEAREST 0x2600
	ETF_Linear = 0x2601,// GL_LINEAR 0x2601
	EM_NearestMipmapNearest = 0x2700,//GL_NEAREST_MIPMAP_NEAREST 0x2700
	EM_LinearMipmapNearest = 0x2701,// GL_LINEAR_MIPMAP_NEAREST 0x2701
	EM_NearestMipmapLinear = 0x2702,// GL_NEAREST_MIPMAP_LINEAR 0x2702
	EM_LinearMipmapLinear = 0x2703//GL_LINEAR_MIPMAP_LINEAR 0x2703
};


/**
* @brief 纹理环绕方式定义
* 环绕方式				描述
* GL_REPEAT				对纹理的默认行为。重复纹理图像。
* GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
* GL_CLAMP_TO_EDGE		纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
* GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
*/
enum ETexAround
{
	ETA_Repeat = 0x2901,//GL_REPEAT 0x2901
	ETA_MirroredRepeat = 0x8370,// GL_MIRRORED_REPEAT 0x8370
	ETA_ClampToEdge = 0x812F,//GL_CLAMP_TO_EDGE 0x812F
	ETA_ClampToBorder = 0x812D// GL_CLAMP_TO_BORDER 0x812D
};


struct Texture2DInfo
{
	unsigned texHD;
	int width = 800;
	int height = 600;
	int channel;
};

/** @breif 该结构体既作为输入（缓冲宽高），又作为输出（句柄）*/
struct HDRFramebufferParam
{
	//输入
	int width = 800;
	int height = 600;

	//输出
	unsigned hdr_fbo;//HDR帧缓冲
	unsigned hdr_depthbuffer;//深度缓冲
	unsigned hdr_colorbuffer[2];//颜色缓冲附件	

	unsigned pingpong_fbo[2];//这两个帧缓冲分别一个颜色附件
	unsigned pingpong_colorbuffer[2];
};

struct MSFramebufferParam
{
	//输入
	int width = 800;
	int height = 600;
	int sample = 4;

	unsigned ms_fbo;
	unsigned ms_colorbuffer1;//
	unsigned ms_colorbuffer2;//高亮片段提取
	unsigned ms_depth_attachment;
};

struct PingPongFramebufferParam
{
	//输入
	int width = 800;
	int height = 600;

	unsigned pingpong_fbo[2];//这两个帧缓冲分别一个颜色附件
	unsigned pingpong_colorbuffer[2];
};


struct ColorFramebufferParam
{
	//输入
	int width = 800;
	int height = 600;

	unsigned color_fbo;
	unsigned color_buffer;
};

/**
 * @brief 纹理的创建、绑定、删除
 * 可能会管理帧缓冲的创建和绑定，因为帧缓冲其实就是一个纹理（颜色二维数组）
 */
class CeTextureFormatter
{

public:
	//创建纹理
	
	/**
	* @brief 创建一个2D纹理，返回该纹理的Handle
	* @param aData	纹理数据
	* @param aWidth 纹理宽度
	* @param aHeight 纹理高度
	* @param aChannels 纹理颜色通道数量
	* @return 纹理信息
	*/
	static Texture2DInfo CreateTexture2D(unsigned char* aData, int aWidth, int aHeight, int aChannels,
		ETexAround aTextureAroundS = ETexAround::ETA_Repeat,
		ETexAround aTextureAroundT = ETexAround::ETA_Repeat,
		ETexFilter aTextureFilterMIN = ETexFilter::ETF_Nearest,
		ETexFilter aTextureFilterMGA = ETexFilter::ETF_Nearest);
 
	/**
	 * @breif 创建一个立方体纹理（天空盒子）
	 * @detail 纹理目标	方位		
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_X	右
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_X	左
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Y	上
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	下
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Z	后
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	前
	 */
	static unsigned CreateTextureCube(int aWidth,int aHeight,int aChannals, vector<unsigned char*>& aFaces);

public:
	//创建帧缓冲	

	/** @breif 创建一个帧缓冲对象并返回其句柄*/
	static FBOID CreateFramebuffer();

	/** @breif 创建一个附加了一个颜色附件的帧缓冲*/
	static unsigned CreateColorFramebuffer(ColorFramebufferParam& aParam);
	
public:
	//创建绑定了特定附件的帧缓冲

	/**
	 * @breif 用于泛光和HDR（High Dynamic Range,即颜色值不是0到1，可以大于1）的浮点帧缓冲
	 * @note 用于HDR需要更高精度的内部数据精度，即GL_RGB16
	 */
	static void CreateFloatFrameBuffer(TextureParam aParam, unsigned& aFBOHD, unsigned aTBONum, vector<unsigned>& aTBOs);

	/**
	 * @breif 该方法创建了一个HDR（High Dynamic Range）帧缓冲
	 * @detail HDR帧缓冲用于HDR光照效果和泛光效果，这两种效果需要操作超过1的颜色值，然后映射到0~1
	 * 这个帧缓冲包含3个附件，两个颜色缓冲附件和一个深度渲染缓冲附件
	 * 还有两个跳跃帧缓冲用于高斯模糊效果生成
	 */
	static void CreateHRDFramebuffer(HDRFramebufferParam& aParam);

	/**
	 * @breif 创建一个带有深度缓冲附件的帧缓冲，无颜色附件
	 * 可用于渲染深度贴图（可用在着色器中采样，渲染缓冲不能用在着色器中采样），阴影映射
	 */
	static void CreateFrambufferWithDepthTexture(FBOParam& aParam,unsigned& aTexID);

public:
	//创建帧缓冲附件

	/**
	 * @breif 创建一个纹理帧缓冲,返回附加了这个纹理附件的帧缓冲对象，实际上就是创建了一个空纹理
	 * @detail 帧缓冲对象绑定了一个纹理。双缓冲的缓冲就是这个吧
	 * @note 注意一个帧缓冲只需要一个纹理或者一个渲染缓冲对象！
	 */
	static unsigned CreateTextureBuffer(TextureParam aTextureParam, int aAttachment = 0);

	//创建一个用于渲染深度贴图的纹理
	static TextureFrameBufferHd CreateDepthTextureBuffer(TextureParam aTextureParam);	

	//创建一个渲染缓冲对象,返回附加了这个渲染缓冲附件的帧缓冲对象
	static TextureFrameBufferHd CreateRenderBuffer(RenderBufferParam aRenderBufferParam);

	/**
	 * @breif 创建一个深度渲染缓冲。
	 * @detail 对于自定义的帧缓冲，如果没有一个深度渲染缓冲附件绑定到这个帧缓冲，那么深度测试
	 * 就不会成功执行。成功创建一个深度渲染缓冲之后，需要绑定到一个帧缓冲。
	 */
	static unsigned CreateDepthRenderBuffer(unsigned aFBO,unsigned aWidth,unsigned aHeight);

public:
	//泛光

	/** @breif 创建两个帧缓冲，渲染的时候来回切换帧缓冲绑定，来对渲染过的图像进行处理。可用于实现模糊效果*/
	static void CreatePingpongFramebuffers(PingPongFramebufferParam& aParam);
public:
	//多重采样

	//多重采样帧缓冲
	static unsigned CreateMSColorbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum, int aAttachment = 0);

	//创建一个多重采样的颜色缓冲
	static TextureFrameBufferHd CreateMSRenderBuffer(RenderBufferParam aRenderBufferParam, unsigned aSampleNum);

	//创建一个多重采样的深度缓冲
	static unsigned CreateMSDepthRenderbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum);

	//创建一个带多重采样颜色缓冲和多重采样深度缓冲的帧缓冲
	static unsigned CreateMSFramebuffer(MSFramebufferParam& aParam);


public:
	//绑定纹理

	/**
	 * @breif 在材质编译的时候给着色器绑定的纹理
	 * @param aShaderID 着色器句柄
	 * @param aTexID 纹理句柄
	 * @param aTexName 纹理名字
	 * @param aTexUnit 纹理单元
	 * @param aTexTarget 纹理目标，默认为GL_TEXTURE_2D
	 */
	static unsigned BindTexture2D(unsigned aShaderID, unsigned aTexID, unsigned aTexUnit = 0,
		unsigned aTexTarget = 0x0DE1);// GL_TEXTURE_2D 0x0DE1

	/** @breif 带偏移的多纹理绑定*/
	static void BindTexture2D(unsigned aShaderIDs,  const vector<unsigned>& aTexIDs,
		unsigned aTexUnitOffset = 0,unsigned aTexTarget = 0x0DE1);

	/** @breif 使用默认纹理单元且只绑定一个纹理*/
	static void BindTexture2D(unsigned aTexID, int aTexUnit = 0);

	static void BindTextureCube( unsigned aTexHD);

	/** @breif 绑定附件到帧缓冲先启用帧缓冲*/
	static int BindAttachmentToFrameBuffer(unsigned aFrameBufferID, unsigned aAttachmentID, unsigned aAtttchmentTag = 0);

	static void BindFramebuffer(unsigned aFrameBufferID);

	/** @breif 绑定帧缓冲，并将视口大小调整为帧缓冲的分辨率*/
	static void BindFramebuffer(unsigned aFBO, unsigned aWidth, unsigned aHeight);

	static void UnbindFrameBuffer();

	static void UnbindTexture2D();

	static bool CheckFramebuffer(unsigned aFBO);

	/** @breif 将一个颜色缓冲从一个帧缓冲复制到另一个帧缓冲*/
	static void ExchangeColorbuffer(unsigned aFBO, unsigned aTargetFBO, unsigned aWidth, unsigned aHeight, unsigned aAttachmentNum);

	/** 删除单个纹理*/
	static void DeleteTexture2D(unsigned aTexID);

	/** 删除多个纹理*/
	static void DeleteTexture2D(const vector<unsigned>& aTexIDs);


public:
	//清除帧缓冲

	static void ClearDepthbuffer();

	static void ClearColorbffer();

	static void ClearStencilbuffer();

	static void ClearColorAndDepthbuffer();

	static void ClearAllbuffer();

};