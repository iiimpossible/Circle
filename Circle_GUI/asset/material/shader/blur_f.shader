#version 330 core
out vec4 oFragColor;
in vec2 oTexCoord;

uniform sampler2D iImage;

uniform bool iHorizontal;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
//uniform float weight[5] = float[](0.016216, 0.054054, 0.1216216, 0.1945946, 0.227027);

void main()
{
	vec2 tex_offset = 1.0 / textureSize(iImage, 0); // gets size of single texel ������������صĴ�С��
	vec3 result = texture(iImage, oTexCoord).rgb * weight[0]; // ��ǰƬ�εĹ���	
	//������˹ģ������CPU�п���
	if (iHorizontal)
	{
		for (int i = 1; i < 5; ++i)
		{
			//ȡ���ұ�4����λ���������Ȼ�����Ȩ��
			result += texture(iImage, oTexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			//ȡ�����4����λ���������
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