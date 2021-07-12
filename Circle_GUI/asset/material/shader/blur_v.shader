#version 330 core
layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexCoord;

out vec2 oTexCoord;


void main()
{
	oTexCoord = iTexCoord;
	//计算片段位置，即将片段坐标通过模型矩阵转换到模型空间	
	gl_Position = vec4(iPos.x, iPos.y, 0.0, 1.0);
	//将片段位置转换到平行光源的位置
	//FragPosInLightSpace = light_projection * light_view * vec4(FragPos, 1.0f);
}