
#version 330 core
in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;
layout(location = 0) out vec4 oFragColor;
layout(location = 1) out vec4 oBrightColor;
 


uniform float iTime;
uniform vec3 iEmissive;

void main()
{
	oFragColor = vec4(iEmissive, 1.0)*2.0;
	float brightness = dot(oFragColor.rgb, vec3(0.5126, 0.7152, 0.5722));//这个是什么原理？求oFragColor在这个向量上的投影？
	if (brightness > 1.0)
		oBrightColor = vec4(oFragColor.rgb, 1.0);	
}

 
