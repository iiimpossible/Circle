#version 330 core
out vec4 oFragColor;
in vec2 oTexCoord;

uniform sampler2D iImage;

uniform bool iHorizontal;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
//uniform float weight[5] = float[](0.016216, 0.054054, 0.1216216, 0.1945946, 0.227027);

void main()
{
	vec2 tex_offset = 1.0 / textureSize(iImage, 0); // gets size of single texel 获得纹理单个像素的大小？
	vec3 result = texture(iImage, oTexCoord).rgb * weight[0]; // 当前片段的贡献	
	//两步高斯模糊，在CPU中控制
	if (iHorizontal)
	{
		for (int i = 1; i < 5; ++i)
		{
			//取样右边4个单位距离的像素然后乘以权重
			result += texture(iImage, oTexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			//取样左边4个单位距离的像素
			result += texture(iImage, oTexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(iImage, oTexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(iImage, oTexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	oFragColor = vec4(result, 1.0);
}