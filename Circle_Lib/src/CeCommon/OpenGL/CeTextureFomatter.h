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

	//����ͨ��������ɫ����
	ETF_RGB = 0x1907,
	ETF_RGBA = 0x1908,
	ETF_RGB16 = 0x8054,
	ETF_RGBA16 = 0x805B,

	//����Ŀ��
	ETF_Texture2D = 0x0DE1,//��ͨ2D����
	ETF_Texture3D = 0x806F,//3D����
	ETF_TextureCubeMap = 0x8513,//��պ���
	ETF_Texture2DMultiSample = 0x9100,//���ز���2D����

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
* @brief ������˷�ʽ
* GL_NEAREST��Ҳ���ڽ����ˣ�Nearest Neighbor Filtering����OpenGLĬ�ϵ�������˷�ʽ��
* ������ΪGL_NEAREST��ʱ��OpenGL��ѡ�����ĵ���ӽ�����������Ǹ����ء���ͼ������Կ���
* �ĸ����أ��ӺŴ����������ꡣ���Ͻ��Ǹ��������ص����ľ�����������������������ᱻѡ��Ϊ��
* ����ɫ��
* GL_LINEAR��Ҳ�����Թ��ˣ�(Bi)linear Filtering����������������긽�����������أ������һ
* ����ֵ�����Ƴ���Щ��������֮�����ɫ��һ���������ص����ľ�����������Խ������ô�����������
* ����ɫ�����յ�������ɫ�Ĺ���Խ��

* �༶��Զ����
* ���˷�ʽ						����
* GL_NEAREST_MIPMAP_NEAREST	ʹ�����ڽ��Ķ༶��Զ������ƥ�����ش�С����ʹ���ڽ���ֵ�����������
* GL_LINEAR_MIPMAP_NEAREST	ʹ�����ڽ��Ķ༶��Զ�����𣬲�ʹ�����Բ�ֵ���в���
* GL_NEAREST_MIPMAP_LINEAR	��������ƥ�����ش�С�Ķ༶��Զ����֮��������Բ�ֵ��ʹ���ڽ���ֵ���в���
* GL_LINEAR_MIPMAP_LINEAR	�������ڽ��Ķ༶��Զ����֮��ʹ�����Բ�ֵ����ʹ�����Բ�ֵ���в���
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
* @brief �����Ʒ�ʽ����
* ���Ʒ�ʽ				����
* GL_REPEAT				�������Ĭ����Ϊ���ظ�����ͼ��
* GL_MIRRORED_REPEAT	��GL_REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
* GL_CLAMP_TO_EDGE		��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
* GL_CLAMP_TO_BORDER	����������Ϊ�û�ָ���ı�Ե��ɫ��
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

/** @breif �ýṹ�����Ϊ���루�����ߣ�������Ϊ����������*/
struct HDRFramebufferParam
{
	//����
	int width = 800;
	int height = 600;

	//���
	unsigned hdr_fbo;//HDR֡����
	unsigned hdr_depthbuffer;//��Ȼ���
	unsigned hdr_colorbuffer[2];//��ɫ���帽��	

	unsigned pingpong_fbo[2];//������֡����ֱ�һ����ɫ����
	unsigned pingpong_colorbuffer[2];
};

struct MSFramebufferParam
{
	//����
	int width = 800;
	int height = 600;
	int sample = 4;

	unsigned ms_fbo;
	unsigned ms_colorbuffer1;//
	unsigned ms_colorbuffer2;//����Ƭ����ȡ
	unsigned ms_depth_attachment;
};

struct PingPongFramebufferParam
{
	//����
	int width = 800;
	int height = 600;

	unsigned pingpong_fbo[2];//������֡����ֱ�һ����ɫ����
	unsigned pingpong_colorbuffer[2];
};


struct ColorFramebufferParam
{
	//����
	int width = 800;
	int height = 600;

	unsigned color_fbo;
	unsigned color_buffer;
};

/**
 * @brief ����Ĵ������󶨡�ɾ��
 * ���ܻ����֡����Ĵ����Ͱ󶨣���Ϊ֡������ʵ����һ��������ɫ��ά���飩
 */
class CeTextureFormatter
{

public:
	//��������
	
	/**
	* @brief ����һ��2D�������ظ������Handle
	* @param aData	��������
	* @param aWidth ������
	* @param aHeight ����߶�
	* @param aChannels ������ɫͨ������
	* @return ������Ϣ
	*/
	static Texture2DInfo CreateTexture2D(unsigned char* aData, int aWidth, int aHeight, int aChannels,
		ETexAround aTextureAroundS = ETexAround::ETA_Repeat,
		ETexAround aTextureAroundT = ETexAround::ETA_Repeat,
		ETexFilter aTextureFilterMIN = ETexFilter::ETF_Nearest,
		ETexFilter aTextureFilterMGA = ETexFilter::ETF_Nearest);
 
	/**
	 * @breif ����һ��������������պ��ӣ�
	 * @detail ����Ŀ��	��λ		
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_X	��
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_X	��
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Y	��
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	��
	 * GL_TEXTURE_CUBE_MAP_POSITIVE_Z	��
	 * GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	ǰ
	 */
	static unsigned CreateTextureCube(int aWidth,int aHeight,int aChannals, vector<unsigned char*>& aFaces);

public:
	//����֡����	

	/** @breif ����һ��֡������󲢷�������*/
	static FBOID CreateFramebuffer();

	/** @breif ����һ��������һ����ɫ������֡����*/
	static unsigned CreateColorFramebuffer(ColorFramebufferParam& aParam);
	
public:
	//���������ض�������֡����

	/**
	 * @breif ���ڷ����HDR��High Dynamic Range,����ɫֵ����0��1�����Դ���1���ĸ���֡����
	 * @note ����HDR��Ҫ���߾��ȵ��ڲ����ݾ��ȣ���GL_RGB16
	 */
	static void CreateFloatFrameBuffer(TextureParam aParam, unsigned& aFBOHD, unsigned aTBONum, vector<unsigned>& aTBOs);

	/**
	 * @breif �÷���������һ��HDR��High Dynamic Range��֡����
	 * @detail HDR֡��������HDR����Ч���ͷ���Ч����������Ч����Ҫ��������1����ɫֵ��Ȼ��ӳ�䵽0~1
	 * ���֡�������3��������������ɫ���帽����һ�������Ⱦ���帽��
	 * ����������Ծ֡�������ڸ�˹ģ��Ч������
	 */
	static void CreateHRDFramebuffer(HDRFramebufferParam& aParam);

	/**
	 * @breif ����һ��������Ȼ��帽����֡���壬����ɫ����
	 * ��������Ⱦ�����ͼ����������ɫ���в�������Ⱦ���岻��������ɫ���в���������Ӱӳ��
	 */
	static void CreateFrambufferWithDepthTexture(FBOParam& aParam,unsigned& aTexID);

public:
	//����֡���帽��

	/**
	 * @breif ����һ������֡����,���ظ����������������֡�������ʵ���Ͼ��Ǵ�����һ��������
	 * @detail ֡����������һ������˫����Ļ�����������
	 * @note ע��һ��֡����ֻ��Ҫһ���������һ����Ⱦ�������
	 */
	static unsigned CreateTextureBuffer(TextureParam aTextureParam, int aAttachment = 0);

	//����һ��������Ⱦ�����ͼ������
	static TextureFrameBufferHd CreateDepthTextureBuffer(TextureParam aTextureParam);	

	//����һ����Ⱦ�������,���ظ����������Ⱦ���帽����֡�������
	static TextureFrameBufferHd CreateRenderBuffer(RenderBufferParam aRenderBufferParam);

	/**
	 * @breif ����һ�������Ⱦ���塣
	 * @detail �����Զ����֡���壬���û��һ�������Ⱦ���帽���󶨵����֡���壬��ô��Ȳ���
	 * �Ͳ���ɹ�ִ�С��ɹ�����һ�������Ⱦ����֮����Ҫ�󶨵�һ��֡���塣
	 */
	static unsigned CreateDepthRenderBuffer(unsigned aFBO,unsigned aWidth,unsigned aHeight);

public:
	//����

	/** @breif ��������֡���壬��Ⱦ��ʱ�������л�֡����󶨣�������Ⱦ����ͼ����д���������ʵ��ģ��Ч��*/
	static void CreatePingpongFramebuffers(PingPongFramebufferParam& aParam);
public:
	//���ز���

	//���ز���֡����
	static unsigned CreateMSColorbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum, int aAttachment = 0);

	//����һ�����ز�������ɫ����
	static TextureFrameBufferHd CreateMSRenderBuffer(RenderBufferParam aRenderBufferParam, unsigned aSampleNum);

	//����һ�����ز�������Ȼ���
	static unsigned CreateMSDepthRenderbuffer(unsigned aWidth, unsigned aHeight, unsigned aSampleNum);

	//����һ�������ز�����ɫ����Ͷ��ز�����Ȼ����֡����
	static unsigned CreateMSFramebuffer(MSFramebufferParam& aParam);


public:
	//������

	/**
	 * @breif �ڲ��ʱ����ʱ�����ɫ���󶨵�����
	 * @param aShaderID ��ɫ�����
	 * @param aTexID ������
	 * @param aTexName ��������
	 * @param aTexUnit ����Ԫ
	 * @param aTexTarget ����Ŀ�꣬Ĭ��ΪGL_TEXTURE_2D
	 */
	static unsigned BindTexture2D(unsigned aShaderID, unsigned aTexID, unsigned aTexUnit = 0,
		unsigned aTexTarget = 0x0DE1);// GL_TEXTURE_2D 0x0DE1

	/** @breif ��ƫ�ƵĶ������*/
	static void BindTexture2D(unsigned aShaderIDs,  const vector<unsigned>& aTexIDs,
		unsigned aTexUnitOffset = 0,unsigned aTexTarget = 0x0DE1);

	/** @breif ʹ��Ĭ������Ԫ��ֻ��һ������*/
	static void BindTexture2D(unsigned aTexID, int aTexUnit = 0);

	static void BindTextureCube( unsigned aTexHD);

	/** @breif �󶨸�����֡����������֡����*/
	static int BindAttachmentToFrameBuffer(unsigned aFrameBufferID, unsigned aAttachmentID, unsigned aAtttchmentTag = 0);

	static void BindFramebuffer(unsigned aFrameBufferID);

	/** @breif ��֡���壬�����ӿڴ�С����Ϊ֡����ķֱ���*/
	static void BindFramebuffer(unsigned aFBO, unsigned aWidth, unsigned aHeight);

	static void UnbindFrameBuffer();

	static void UnbindTexture2D();

	static bool CheckFramebuffer(unsigned aFBO);

	/** @breif ��һ����ɫ�����һ��֡���帴�Ƶ���һ��֡����*/
	static void ExchangeColorbuffer(unsigned aFBO, unsigned aTargetFBO, unsigned aWidth, unsigned aHeight, unsigned aAttachmentNum);

	/** ɾ����������*/
	static void DeleteTexture2D(unsigned aTexID);

	/** ɾ���������*/
	static void DeleteTexture2D(const vector<unsigned>& aTexIDs);


public:
	//���֡����

	static void ClearDepthbuffer();

	static void ClearColorbffer();

	static void ClearStencilbuffer();

	static void ClearColorAndDepthbuffer();

	static void ClearAllbuffer();

};