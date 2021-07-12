#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec3 TexCoords;

uniform samplerCube iSkybox;

void main()
{
	FragColor = texture(iSkybox, TexCoords);
	BrightColor = vec4(0.0);
}
