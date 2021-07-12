
#version 330 core
in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;
in vec4 oFragPosLightSpace;

layout(location = 0) out vec4 oFragColor;
layout(location = 1) out vec4 oBrightColor;



#define NR_POINT_LIGHTS 10
struct PointLight
{
	vec3 color;
	vec3 position;
};


uniform int iLightNum;
uniform PointLight PointLights[NR_POINT_LIGHTS];

uniform float iTime;
uniform vec3 iCameraPos;
uniform vec3 iAmbientColor;


uniform sampler2D iDiffuse;
uniform sampler2D iSpecular;
uniform sampler2D iShadowMap;


vec3 CaculateDirLight(PointLight aLight, vec3  aFragPos, vec3 aNormal);

float CaculateShadow(vec4 aFragPosInLightSpace, vec3 aNormal, vec3 aLightDir);

vec3 CaculateSpecularLight(PointLight aLight, vec3 aCameraPos, vec3 aFragPos, vec3 aNormal);

void main()
{

	vec3 nor = normalize(oNormal);
	vec3 light_dir = normalize(PointLights[0].position - oFragPos);

	vec3 diffuse = vec3(texture(iDiffuse, oTexCoord));
	vec3 specular = CaculateSpecularLight(PointLights[0],iCameraPos,oFragPos,nor);

	vec3 light;
	
	//��ʱ�ļ�¼�ܲ�����Ӱ���Ǹ���Դ���Ժ�Ҫ��
	vec3 sun_light = CaculateDirLight(PointLights[0], oFragPos, nor);
	for (int i = 1; i < iLightNum && i< NR_POINT_LIGHTS; i++)
	{		
		light += CaculateDirLight(PointLights[i], oFragPos, nor);
	}

	//������Ӱϵ��������Ӱ�����1.0�������0.0
	float shadow = CaculateShadow(oFragPosLightSpace, nor, light_dir);

	vec3 temp_ambient = vec3(0.35);
	//vec3 res = (iAmbientColor +  sun_light )  * diffuse;
	//vec3 res = (iAmbientColor + (1.0-shadow)* sun_light + light)  * diffuse + specular;//�����л�����ͱ�
	vec3 res = (temp_ambient + sun_light  + light)  * diffuse + specular;//�����л�����ͱ�


	oFragColor = vec4(res, 1.0);
	float brightness = dot(oFragColor.rgb, vec3(0.2126, 0.7152, 0.0722));//�����ʲôԭ����oFragColor����������ϵ�ͶӰ��
	if (brightness > 2.0)
		oBrightColor = vec4(oFragColor.rgb, 1.0);
}


vec3 CaculateDirLight(PointLight aLight, vec3  aFragPos, vec3 aNormal)
{
	vec3 lightDir = normalize(aLight.position - aFragPos);
	float diff = max(dot(lightDir, aNormal), 0.0);
	vec3 res = diff * aLight.color;
	return res;
}

vec3 CaculateSpecularLight(PointLight aLight, vec3 aCameraPos, vec3 aFragPos, vec3 aNormal)
{
	vec3 lightDir = normalize(aLight.position - aFragPos);
	vec3 viewDir = normalize(aCameraPos - aFragPos);
	vec3 reflectDir = reflect(-lightDir, aNormal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular =  spec * aLight.color;
	return specular;
}


float CaculateShadow(vec4 aFragPosInLightSpace, vec3 aNormal, vec3 aLightDir)
{
	//ִ��͸�ӳ�����ʵ���ϣ�ͶӰ����û��ֱ�ӽ���Ϸ����Ķ���任��ɽ���ԶС��͸��Ч����ֻ�ǽ����嶥���w��������Ϊһ�����ʵ�ֵ��
	//OpenGL���Զ������㣨gl_Postion������w��������������OpenGL��û��������������
	//��ͶӰ���������������ʱ��w����Ϊ1��ֻ��ͶӰ������͸�ӵ������w��������
	vec3 projCoord = aFragPosInLightSpace.rgb / aFragPosInLightSpace.w;

	//NDC�豸����ϵ�任����[-1,1]�任��[0,1]
	projCoord = projCoord * 0.5 + 0.5;

	//������Դλ�����������,���뽫ƬԪλ��ת������ռ�ſ�����ȷ����
	float closestDepth = texture(iShadowMap, projCoord.xy).r;

	// ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
	float currentDepth = projCoord.z;

	// ��鵱ǰƬ���Ƿ�����Ӱ��
	float bias = max(0.05 * (1.0 - dot(aNormal, aLightDir)), 0.005);//�����Դ���䵽�������ķ��򣬴Ӷ���̬ȷ����Ӱƫ��ֵ������Ħ����
//	float shadow = currentDepth - bias > closestDepth ? 0.8 : 0.0;



	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(iShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(iShadowMap, projCoord.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 0.8 : 0.0;
		}
	}
	shadow /= 9.0;


	if (projCoord.z > 1.0)
		shadow = 0.0;

	return shadow;
}


