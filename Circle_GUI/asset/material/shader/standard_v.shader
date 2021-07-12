#version 330 core
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexCoord;

out vec2 oTexCoord;
out vec3 oNormal;
out vec3 oFragPos;
out vec4 oFragPosLightSpace;

uniform mat4 iLightView;
uniform mat4 iLightProj;

uniform mat4 iModel;
uniform mat4 iView;
uniform mat4 iProjection;
uniform float iTime;

void main()
{
	oTexCoord = iTexCoord;
	oNormal = mat3(transpose(inverse(iModel))) * iNormal;//将法线变换到模型空间
	//计算片段位置，即将片段坐标通过模型矩阵转换到模型空间
	oFragPos = vec3(iModel * vec4(iPos, 1.0f));

	oFragPosLightSpace = iLightProj * iLightView * vec4(oFragPos, 1.0f);
	gl_Position = iProjection * iView * iModel * vec4(iPos, 1.0f);
	//将片段位置转换到平行光源的位置
	//FragPosInLightSpace = light_projection * light_view * vec4(FragPos, 1.0f);
}