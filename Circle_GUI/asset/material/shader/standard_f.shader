
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
	
	//临时的记录能产生阴影的那个光源，以后要改
	vec3 sun_light = CaculateDirLight(PointLights[0], oFragPos, nor);
	for (int i = 1; i < iLightNum && i< NR_POINT_LIGHTS; i++)
	{		
		light += CaculateDirLight(PointLights[i], oFragPos, nor);
	}

	//计算阴影系数，在阴影中输出1.0，在输出0.0
	float shadow = CaculateShadow(oFragPosLightSpace, nor, light_dir);

	vec3 temp_ambient = vec3(0.35);
	//vec3 res = (iAmbientColor +  sun_light )  * diffuse;
	//vec3 res = (iAmbientColor + (1.0-shadow)* sun_light + light)  * diffuse + specular;//至少有环境光低保
	vec3 res = (temp_ambient + sun_light  + light)  * diffuse + specular;//至少有环境光低保


	oFragColor = vec4(res, 1.0);
	float brightness = dot(oFragColor.rgb, vec3(0.2126, 0.7152, 0.0722));//这个是什么原理？求oFragColor在这个向量上的投影？
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
	//执行透视除法。实际上，投影矩阵并没有直接将游戏物体的顶点变换造成近大远小的透视效果，只是将物体顶点的w分量计算为一个合适的值，
	//OpenGL会自动将顶点（gl_Postion）除以w分量，而在这里OpenGL并没有替我们做除法
	//当投影矩阵是正交矩阵的时候w分量为1，只有投影矩阵是透视的情况下w才有意义
	vec3 projCoord = aFragPosInLightSpace.rgb / aFragPosInLightSpace.w;

	//NDC设备坐标系变换。从[-1,1]变换到[0,1]
	projCoord = projCoord * 0.5 + 0.5;

	//获得离光源位置最近点的深度,必须将片元位置转换到光空间才可以正确采样
	float closestDepth = texture(iShadowMap, projCoord.xy).r;

	// 取得当前片段在光源视角下的深度
	float currentDepth = projCoord.z;

	// 检查当前片段是否在阴影中
	float bias = max(0.05 * (1.0 - dot(aNormal, aLightDir)), 0.005);//计算光源照射到物体表面的方向，从而动态确定阴影偏移值，避免摩尔纹
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


