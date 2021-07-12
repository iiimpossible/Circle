#version 330 core
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormals;
layout(location = 2) in vec2 iTexCoords;

//uniform mat4 iLightSpace;

uniform mat4 iModel;
uniform mat4 iView;
uniform mat4 iProjection;

out vec3 oFragpos;

void main()
{
	oFragpos = vec3(iModel * vec4(iPos, 1.0f));
	gl_Position = iProjection *  iView* iModel * vec4(iPos, 1.0f);
}