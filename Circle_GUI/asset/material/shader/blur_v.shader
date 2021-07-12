#version 330 core
layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexCoord;

out vec2 oTexCoord;


void main()
{
	oTexCoord = iTexCoord;
	//����Ƭ��λ�ã�����Ƭ������ͨ��ģ�;���ת����ģ�Ϳռ�	
	gl_Position = vec4(iPos.x, iPos.y, 0.0, 1.0);
	//��Ƭ��λ��ת����ƽ�й�Դ��λ��
	//FragPosInLightSpace = light_projection * light_view * vec4(FragPos, 1.0f);
}