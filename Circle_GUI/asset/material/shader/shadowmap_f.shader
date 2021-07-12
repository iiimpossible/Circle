#version 330 core

in vec3 oFragpos;
void main()
{
	//下边这行可有可无
	//gl_FragDepth = oFragpos.z;
	//没有颜色缓冲该片段着色器不做任何处理
}