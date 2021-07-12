#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 iProjection;
uniform mat4 iView;

void main()
{
	TexCoords = aPos;
	gl_Position = iProjection * iView * vec4(aPos, 1.0);
}