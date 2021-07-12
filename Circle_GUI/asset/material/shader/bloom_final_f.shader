#version 330 core
out vec4 oFragColor;

in vec2 TexCoords;

//����������Ⱦ���
uniform sampler2D iScene;
//������ģ�����
uniform sampler2D iBloom;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	const float brightness = 0.3;
	vec3 scene = texture(iScene, TexCoords).rgb;
	vec3 bloom = texture(iBloom, TexCoords).rgb;

	//����ģ������Ӧ�����Ѿ����й��ý����ˣ�����ֻ�Ը���������Цý���
	bloom = pow(bloom, vec3(1.0 / gamma));
	scene += bloom; // ��ɫ��ӻ��
	
	//ɫ��ӳ�䣬�ؼ�exp������Ȼ����Ϊ�׵�ָ������
	//��sceneȡ������scene����[0,��],�Ӷ�-scene[-��.0,0]
	//�Ӷ�exp(-scene)����[-��,1]��Ȼ����1.0��ȥǰ�棬�Ӷ�ӳ�䵽[0,1]
	//brightness����������������ɫʧ��̶�
	vec3 result = vec3(1.0) - exp(-scene * brightness);	
	oFragColor = vec4(result/ (brightness*0.9), 1.0);
}