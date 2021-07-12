#pragma once
#include <vector>
#include <string>
#include <Celib_x64/src/CeCommon/Math/Math.h>
using namespace CircleEngine;


/**
 * @breif 着色器元素，包含一个着色器所需的必要信息
 * @property texture_name 绑定到该着色器的纹理的名字数组
 * @property texutre_hd 绑定到该着色器的纹理的句柄
 * @property shader_hd 着色器句柄
 * @property diffuse 漫反射强度
 * @property specular 高光反射强度
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
 * 包含各种功能组件所需的各种结构体，未来数量多起来可考虑分开
 */

/**
 * 渲染的信息，包括着色器句柄，纹理句柄和顶点数组句柄
 * @note 注意顺序是着色器->顶点数组->纹理->索引
 * @member 着色器句柄，顶点数组句柄，纹理句柄，顶点索引句柄
 * @note 可能有多个的：EBO TexHD,VAO
 */
struct RenderInfo
{	
	std::vector<unsigned> shader_hds;//着色器
	std::vector<unsigned> ebo_hds;//索引
	std::vector<unsigned> vao_hds;//顶点数组对象
	std::vector<unsigned> indices_nums;//这个是通过索引绘制需要知道有多少个索引。一个索引包含2个(2D)或者3个（3D）数据
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
 * @breif 光照命令
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
 * @breif 游戏对象的场景初始化信息
 */
struct ObjectInitInfo
{
	std::string type;
	std::string name;
	std::vector<std::pair<std::string, std::string>> comp_command;
};



/**
 * @breif 相机信息结构体
 * @detail 之前的相机信息都是每个游戏物体传递一次相机信息，
 * 但是实际上，一帧中的相机信息（透视矩阵）
 */
struct CameraInfo
{
	Vec3 cameraPos;
	Mat4 projection;
	Mat4 view;
};

/**
 * @breif 一个帧缓冲参数的结构体。
 * @detail 对于一个自定义的帧缓冲，要正确渲染到视口最基本的
 * 是需要调整视口大小到这个帧缓冲的分辨率，而这个分辨率是在
 * 定义帧缓冲的时候设置的，因此需要保存下来
 */
struct FBOParam
{
	unsigned fbo;
	unsigned width = 800;
	unsigned height = 600;
};