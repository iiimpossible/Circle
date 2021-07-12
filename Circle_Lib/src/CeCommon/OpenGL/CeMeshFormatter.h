#include <vector>
#include <tuple>
using std::vector;
using std::tuple;

/** @param 数组首地址 数组大小 单个数据维度 步幅*/
typedef tuple<float*, unsigned, unsigned, unsigned> VerterxFormat;

/** @param  数组首地址，数组大小*/
typedef tuple<unsigned*, unsigned> EBOFormat;//这个太难用


struct VBODataFormat
{
	float* Array;//数组首地址
	unsigned ArraySize;//数组大小
	unsigned DataDimension;//数据维度，即这个顶点或者纹理坐标是2维还是3维
	unsigned Stride;//步幅，非分批顶点属性步幅与维度不同（顶点和纹理坐标在一个数组），分批顶点属性步幅与维度相同
};

struct EBODataFormat
{
	unsigned * Array;//数组首地址
	unsigned ArraySize;//数组大小
};

/**
 * @brief 索引渲染模式
 * #define GL_DYNAMIC_DRAW 0x88E8
 * #define GL_STATIC_DRAW 0x88E4
 * #define GL_STREAM_DRAW 0x88E0
 */
enum EIndiceDrawMode
{
	EID_Dynamic = 0x88E8,
	EID_Static = 0x88E4,
	EID_Stream = 0x88E0,
};

class CeMeshFormatter
{
public:	
	unsigned FormatMesh(vector<float> aData);

public:	
	static unsigned CreateVAO();

	/**
	 * @brief创建一个EBO，索引缓冲对象
	 * @param aVAO 将这个EBO绑定到这个顶点数组对象
	 * @param aFormat 输入数组首地址以及数组元素个数
	 * @param aDrawMode 缓冲是动态的还是静态的（顶点是否运行时改变）
	 */	 
	static unsigned CreateEBO(unsigned aVAO, EBOFormat aFormat, EIndiceDrawMode aDrawMode = EIndiceDrawMode::EID_Static);
public:
	//分批顶点属性，分批是指第一段数据都是顶点，第二段数据都是纹理坐标，第三段都是法线

	/**
	* @brief 分批顶点属性
	* 数组大小是为了根据这个值首先分配对应大小的显存。单个数据维度是这个数据是1D，2D，还是3D的。
	* 步幅是Opengl读取相应数据的时候，应该以几个float（或者其他单位）为一个基本单元。
	* 当维度和步幅相等的时候，意味着一个数据就是一个基本单元，而维度和步幅不等，意味着数据输入不是分批的，
	* 而是交错的，比如一个顶点一个纹理坐标。那么此时者两种数据有自己的维度，而步幅等于两个维度之和，
	* opengl就按照这个步幅来读取数据
	* @param[in] aVAO 顶点数组对象句柄
	* @param[in] aVertexInfo 顶点数据及其格式信息。按照顺序：数组指针，数组大小，单个数据维度，步幅
	* @param[in] aTexCoordInfo 纹理坐标数组及其信息。
	*/
	static unsigned CreateVBO(unsigned aVAO, VerterxFormat aVertexInfo,VerterxFormat aTexCoordInfo);

	static unsigned CreateVBO(unsigned aVAO, VBODataFormat aVerFormat, VBODataFormat aTexFormat);

	//分批顶点属性，带法向量
	static unsigned CreateVBO(unsigned aVAO, VerterxFormat aVertexInfo, VerterxFormat aTexCoordInfo, VerterxFormat aNormalInfo);


	//static unsigned CreateVAO();
	//static unsigned CreateVBO();
	//static unsigned CreateEBO();

	//分配空显存（动态写入顶点信息，一个VAO实现绘制不同的图案，线段、方块、圆等）

public:
	//动态VAO支持



}; 