#include "InlineData.h"
#include <tuple>
#include <CeCommon/OpenGL/CeMeshFormatter.h>

const char* InlineShaderCode::Line_VS = INLINESHADER(

#version 330 core \n
layout(location = 0) in vec3 position;
uniform mat4 projection;
void main()
{
	gl_Position = projection * vec4(position.x, position.y, 0.0f, 1.0f);
}

);

const char* InlineShaderCode::Line_FS = INLINESHADER(

#version 330 core \n
out vec4 Fragcolor;

uniform vec3 color;
void main()
{
	Fragcolor = vec4(color.x, color.y, color.z, 1.0f);
}

);

//================================================================ 

const char* InlineShaderCode::Rec_VS = INLINESHADER(

#version 330 core \n
layout(location = 0) in vec3 position;
uniform mat4 projection;
void main()
{
	//gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
	gl_Position = projection * vec4(position.x, position.y, 0.0f, 1.0f);
}

);


const char* InlineShaderCode::Rec_FS = INLINESHADER(

#version 330 core \n
out vec4 Fragcolor;

uniform vec3 color;
void main()
{
	Fragcolor = vec4(color.x, color.y, color.z, 1.0f);
}

);

//================================================================ 

const char* InlineShaderCode::PureColor_VS3D = INLINESHADER(
#version 330 core \n
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoords;
layout(location = 2) in vec3 iNormals;

out vec3 Normals;
out vec2 Texcoords;

uniform mat4 iModel;
uniform mat4 iView;
uniform mat4 iProjection;
void main()
{
	Normals = iNormals;
	Texcoords = iTexCoords;
	gl_Position = iProjection * iView * iModel * vec4(iPos, 1.0f);
}
);


const char* InlineShaderCode::PureColor_FS3D = INLINESHADER(
#version 330 core \n
out vec4 Fragcolor;

uniform float iTime;
uniform vec3 iCameraPosition;

void main()
{
	Fragcolor = vec4(0.8, 0.1, 0.5, 1.0f);
}

);

//================================================================ 

const char* InlineShaderCode::PureColor_VS2D = INLINESHADER(
#version 330 core \n
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoords;
uniform mat4 iModel;
uniform mat4 iView;
uniform mat4 iProjection;
void main()
{
	gl_Position = iProjection * iView * iModel * vec4(iPos, 1.0f);
}

);

const char* InlineShaderCode::PureColor_FS2D = INLINESHADER(
#version 330 core \n
out vec4 Fragcolor;

void main()
{
	Fragcolor = vec4(0.8, 0.1, 0.4, 1.0f);
}

);

/*===========================================================================================================*/

const char* InlineShaderCode::Skybox_VS = INLINESHADER(
	#version 330 core\n
	layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 iProjection;
uniform mat4 iView;

void main()
{
	TexCoords = aPos;
	gl_Position = iProjection * iView * vec4(aPos, 1.0);
}

);


const char* InlineShaderCode::Skybox_FS = INLINESHADER(
	#version 330 core\n
	out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube iSkybox;

void main()
{
	FragColor = texture(iSkybox, TexCoords);
}

);

/*===========================================================================================================*/


vector<unsigned> InlineMeshData::Cube_Indices
{
	0,1,3,
	1,2,3,
	4,5,7,
	5,6,7,
	8,9,11,
	9,10,11,
	12,13,15,
	13,14,15,
	16,17,19,
	17,18,19,
	20,21,23,
	21,22,23
};


vector<float> InlineMeshData::Cube_Vertices
{
	// front        
	-0.5f, -0.5f, -0.5f,//0
	 0.5f, -0.5f, -0.5f,//1
	 0.5f,  0.5f, -0.5f,//2
	 //0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,//
	//-0.5f, -0.5f, -0.5f,

	//back
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	// 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	//-0.5f, -0.5f,  0.5f,

	//left	
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,

	//right
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	// 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	// 0.5f,  0.5f,  0.5f,

	//buttom
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	// 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	//-0.5f, -0.5f, -0.5f,

	//top
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	// 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	//-0.5f,  0.5f, -0.5f
};

vector<float> InlineMeshData::Cube_Normals
{
	// normals          
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	//0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	//0.0f,  0.0f, -1.0f,

	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	//0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	//0.0f,  0.0f,  1.0f,

   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   //-1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   //-1.0f,  0.0f,  0.0f,

	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	//1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	//1.0f,  0.0f,  0.0f,

	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	//0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	//0.0f, -1.0f,  0.0f,

	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	//0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	//0.0f,  1.0f,  0.0f
};
vector<float> InlineMeshData::Cube_TexCoords
{
	// texture c
	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f,
	//1.0f,  1.0f,
	0.0f,  1.0f,
//	0.0f,  0.0f,

	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f,
	//1.0f,  1.0f,
	0.0f,  1.0f,
	//0.0f,  0.0f,

	1.0f,  0.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	//0.0f,  1.0f,
	0.0f,  0.0f,
	//1.0f,  0.0f,

	1.0f,  0.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	//0.0f,  1.0f,
	0.0f,  0.0f,
	//1.0f,  0.0f,

	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	//1.0f,  0.0f,
	0.0f,  0.0f,
	//0.0f,  1.0f,

	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	//1.0f,  0.0f,
	0.0f,  0.0f,
	//0.0f,  1.0f
};



vector<float> InlineMeshData::FullSreenRecVertices
{
	1.0,1.0, //右上
	1.0,-1.0,//右下
	-1.0,1.0,//左上
	1.0,-1.0,//右下
	-1.0,-1.0,//左下
	-1.0,1.0//左上
};
vector<float> InlineMeshData::FullSreenRecTexcoods
{
	1.0,1.0,
	1.0,0.0,
	0.0,1.0,
	1.0,0.0,
	0.0,0.0,
	0.0,1.0
};

FullSreenQuad::FullSreenQuad()
{
	mVAO = CeMeshFormatter::CreateVAO();
	mVBO = CeMeshFormatter::CreateVBO(mVAO,
		std::make_tuple(&InlineMeshData::FullSreenRecVertices[0], InlineMeshData::FullSreenRecTexcoods.size(), 2, 2),
		std::make_tuple(&InlineMeshData::FullSreenRecTexcoods[0], InlineMeshData::FullSreenRecTexcoods.size(), 2, 2));
}

FullSreenQuad::~FullSreenQuad()
{
}






//vector<float> CubeWithoutIndices_Vertices
//{
//	// positions        
//	-0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f, -0.5f,
//	 0.5f,  0.5f, -0.5f,
//	 0.5f,  0.5f, -0.5f,
//	-0.5f,  0.5f, -0.5f,
//	-0.5f, -0.5f, -0.5f,
//
//	-0.5f, -0.5f,  0.5f,
//	 0.5f, -0.5f,  0.5f,
//	 0.5f,  0.5f,  0.5f,
//	 0.5f,  0.5f,  0.5f,
//	-0.5f,  0.5f,  0.5f,
//	-0.5f, -0.5f,  0.5f,
//
//	-0.5f,  0.5f,  0.5f,
//	-0.5f,  0.5f, -0.5f,
//	-0.5f, -0.5f, -0.5f,
//	-0.5f, -0.5f, -0.5f,
//	-0.5f, -0.5f,  0.5f,
//	-0.5f,  0.5f,  0.5f,
//
//	 0.5f,  0.5f,  0.5f,
//	 0.5f,  0.5f, -0.5f,
//	 0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f,  0.5f,
//	 0.5f,  0.5f,  0.5f,
//
//	-0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f,  0.5f,
//	 0.5f, -0.5f,  0.5f,
//	-0.5f, -0.5f,  0.5f,
//	-0.5f, -0.5f, -0.5f,
//
//	-0.5f,  0.5f, -0.5f,
//	 0.5f,  0.5f, -0.5f,
//	 0.5f,  0.5f,  0.5f,
//	 0.5f,  0.5f,  0.5f,
//	-0.5f,  0.5f,  0.5f,
//	-0.5f,  0.5f, -0.5f
//};
//
//vector<float>  CubeWithoutIndices_Normals
//{
//	// normals          
//	0.0f,  0.0f, -1.0f,
//	0.0f,  0.0f, -1.0f,
//	0.0f,  0.0f, -1.0f,
//	0.0f,  0.0f, -1.0f,
//	0.0f,  0.0f, -1.0f,
//	0.0f,  0.0f, -1.0f,
//
//	0.0f,  0.0f,  1.0f,
//	0.0f,  0.0f,  1.0f,
//	0.0f,  0.0f,  1.0f,
//	0.0f,  0.0f,  1.0f,
//	0.0f,  0.0f,  1.0f,
//	0.0f,  0.0f,  1.0f,
//
//   -1.0f,  0.0f,  0.0f,
//   -1.0f,  0.0f,  0.0f,
//   -1.0f,  0.0f,  0.0f,
//   -1.0f,  0.0f,  0.0f,
//   -1.0f,  0.0f,  0.0f,
//   -1.0f,  0.0f,  0.0f,
//
//	1.0f,  0.0f,  0.0f,
//	1.0f,  0.0f,  0.0f,
//	1.0f,  0.0f,  0.0f,
//	1.0f,  0.0f,  0.0f,
//	1.0f,  0.0f,  0.0f,
//	1.0f,  0.0f,  0.0f,
//
//	0.0f, -1.0f,  0.0f,
//	0.0f, -1.0f,  0.0f,
//	0.0f, -1.0f,  0.0f,
//	0.0f, -1.0f,  0.0f,
//	0.0f, -1.0f,  0.0f,
//	0.0f, -1.0f,  0.0f,
//
//	0.0f,  1.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,
//	0.0f,  1.0f,  0.0f
//};
//vector<float>  CubeWithoutIndices_TexCoords
//{
//	// texture c
//	0.0f,  0.0f,
//	1.0f,  0.0f,
//	1.0f,  1.0f,
//	1.0f,  1.0f,
//	0.0f,  1.0f,
//	0.0f,  0.0f,
//
//	0.0f,  0.0f,
//	1.0f,  0.0f,
//	1.0f,  1.0f,
//	1.0f,  1.0f,
//	0.0f,  1.0f,
//	0.0f,  0.0f,
//
//	1.0f,  0.0f,
//	1.0f,  1.0f,
//	0.0f,  1.0f,
//	0.0f,  1.0f,
//	0.0f,  0.0f,
//	1.0f,  0.0f,
//
//	1.0f,  0.0f,
//	1.0f,  1.0f,
//	0.0f,  1.0f,
//	0.0f,  1.0f,
//	0.0f,  0.0f,
//	1.0f,  0.0f,
//
//	0.0f,  1.0f,
//	1.0f,  1.0f,
//	1.0f,  0.0f,
//	1.0f,  0.0f,
//	0.0f,  0.0f,
//	0.0f,  1.0f,
//
//	0.0f,  1.0f,
//	1.0f,  1.0f,
//	1.0f,  0.0f,
//	1.0f,  0.0f,
//	0.0f,  0.0f,
//	0.0f,  1.0f
//};