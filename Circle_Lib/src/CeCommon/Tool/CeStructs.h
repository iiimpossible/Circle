#pragma once
#include <vector>
#include <string>
#include <Celib_x64/src/CeCommon/Math/Math.h>
using namespace CircleEngine;


/**
 * @breif ��ɫ��Ԫ�أ�����һ����ɫ������ı�Ҫ��Ϣ
 * @property texture_name �󶨵�����ɫ�����������������
 * @property texutre_hd �󶨵�����ɫ��������ľ��
 * @property shader_hd ��ɫ�����
 * @property diffuse ������ǿ��
 * @property specular �߹ⷴ��ǿ��
 */
struct ShaderElement
{
	unsigned shader_hd;		
	//float diffuse;
	//float specular;
	std::vector<std::string> texture_name;
	std::vector<unsigned> texture_hd;
};


/**
 * �������ֹ����������ĸ��ֽṹ�壬δ�������������ɿ��Ƿֿ�
 */

/**
 * ��Ⱦ����Ϣ��������ɫ��������������Ͷ���������
 * @note ע��˳������ɫ��->��������->����->����
 * @member ��ɫ��������������������������������������
 * @note �����ж���ģ�EBO TexHD,VAO
 */
struct RenderInfo
{	
	std::vector<unsigned> shader_hds;//��ɫ��
	std::vector<unsigned> ebo_hds;//����
	std::vector<unsigned> vao_hds;//�����������
	std::vector<unsigned> indices_nums;//�����ͨ������������Ҫ֪���ж��ٸ�������һ����������2��(2D)����3����3D������
	std::vector<ShaderElement> shader_elements;
	bool bIgnore = false;
};

struct CeMesh
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
	std::vector<unsigned> indices;
};


/**
 * @breif ��������
 */
struct LightCommand
{
	Vec3 lightPos;
	Vec3 lightColor;
	Mat4 lightSpaceMat;	 
	Mat4 lightView;
	Mat4 lightProjection;
};


/**
 * @breif ��Ϸ����ĳ�����ʼ����Ϣ
 */
struct ObjectInitInfo
{
	std::string type;
	std::string name;
	std::vector<std::pair<std::string, std::string>> comp_command;
};



/**
 * @breif �����Ϣ�ṹ��
 * @detail ֮ǰ�������Ϣ����ÿ����Ϸ���崫��һ�������Ϣ��
 * ����ʵ���ϣ�һ֡�е������Ϣ��͸�Ӿ���
 */
struct CameraInfo
{
	Vec3 cameraPos;
	Mat4 projection;
	Mat4 view;
};

/**
 * @breif һ��֡��������Ľṹ�塣
 * @detail ����һ���Զ����֡���壬Ҫ��ȷ��Ⱦ���ӿ��������
 * ����Ҫ�����ӿڴ�С�����֡����ķֱ��ʣ�������ֱ�������
 * ����֡�����ʱ�����õģ������Ҫ��������
 */
struct FBOParam
{
	unsigned fbo;
	unsigned width = 800;
	unsigned height = 600;
};