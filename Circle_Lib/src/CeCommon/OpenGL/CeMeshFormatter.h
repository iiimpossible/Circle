#include <vector>
#include <tuple>
using std::vector;
using std::tuple;

/** @param �����׵�ַ �����С ��������ά�� ����*/
typedef tuple<float*, unsigned, unsigned, unsigned> VerterxFormat;

/** @param  �����׵�ַ�������С*/
typedef tuple<unsigned*, unsigned> EBOFormat;//���̫����


struct VBODataFormat
{
	float* Array;//�����׵�ַ
	unsigned ArraySize;//�����С
	unsigned DataDimension;//����ά�ȣ�����������������������2ά����3ά
	unsigned Stride;//�������Ƿ����������Բ�����ά�Ȳ�ͬ�����������������һ�����飩�������������Բ�����ά����ͬ
};

struct EBODataFormat
{
	unsigned * Array;//�����׵�ַ
	unsigned ArraySize;//�����С
};

/**
 * @brief ������Ⱦģʽ
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
	 * @brief����һ��EBO�������������
	 * @param aVAO �����EBO�󶨵���������������
	 * @param aFormat ���������׵�ַ�Լ�����Ԫ�ظ���
	 * @param aDrawMode �����Ƕ�̬�Ļ��Ǿ�̬�ģ������Ƿ�����ʱ�ı䣩
	 */	 
	static unsigned CreateEBO(unsigned aVAO, EBOFormat aFormat, EIndiceDrawMode aDrawMode = EIndiceDrawMode::EID_Static);
public:
	//�����������ԣ�������ָ��һ�����ݶ��Ƕ��㣬�ڶ������ݶ����������꣬�����ζ��Ƿ���

	/**
	* @brief ������������
	* �����С��Ϊ�˸������ֵ���ȷ����Ӧ��С���Դ档��������ά�������������1D��2D������3D�ġ�
	* ������Opengl��ȡ��Ӧ���ݵ�ʱ��Ӧ���Լ���float������������λ��Ϊһ��������Ԫ��
	* ��ά�ȺͲ�����ȵ�ʱ����ζ��һ�����ݾ���һ��������Ԫ����ά�ȺͲ������ȣ���ζ���������벻�Ƿ����ģ�
	* ���ǽ���ģ�����һ������һ���������ꡣ��ô��ʱ�������������Լ���ά�ȣ���������������ά��֮�ͣ�
	* opengl�Ͱ��������������ȡ����
	* @param[in] aVAO �������������
	* @param[in] aVertexInfo �������ݼ����ʽ��Ϣ������˳������ָ�룬�����С����������ά�ȣ�����
	* @param[in] aTexCoordInfo �����������鼰����Ϣ��
	*/
	static unsigned CreateVBO(unsigned aVAO, VerterxFormat aVertexInfo,VerterxFormat aTexCoordInfo);

	static unsigned CreateVBO(unsigned aVAO, VBODataFormat aVerFormat, VBODataFormat aTexFormat);

	//�����������ԣ���������
	static unsigned CreateVBO(unsigned aVAO, VerterxFormat aVertexInfo, VerterxFormat aTexCoordInfo, VerterxFormat aNormalInfo);


	//static unsigned CreateVAO();
	//static unsigned CreateVBO();
	//static unsigned CreateEBO();

	//������Դ棨��̬д�붥����Ϣ��һ��VAOʵ�ֻ��Ʋ�ͬ��ͼ�����߶Ρ����顢Բ�ȣ�

public:
	//��̬VAO֧��



}; 