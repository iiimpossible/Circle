#version 330 core
out vec4 oFragColor;

in vec2 TexCoords; 
uniform sampler2D iShadowMap;
 

void main()
{ 
	float depth = texture(iShadowMap, TexCoords).r;

	oFragColor = vec4(vec3(depth), 1.0);
}