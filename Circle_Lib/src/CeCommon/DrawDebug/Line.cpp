#include "Line.h"
using CircleEngine::CreateVBOEmpty;
using CircleEngine::ReWriteVBO;
using CircleEngine::CreateVAO;
using CircleEngine::line_fragment_shader;
using CircleEngine::line_vretext_shader;
using CircleEngine::ShaderCompile;
line::line(Vec3 color) :
	Color(color)
{
	VAO = CreateVAO();
	VBO = CreateVBOEmpty(VAO, 4, 2, 2);
	ShaderProgramID = ShaderCompile(line_vretext_shader, line_fragment_shader, nullptr);
}

line::~line()
{
}

float * line::GetIndices()
{
	return EndPosints;
}

void line::CaculateVertices(Vec3 start_point, Vec3 end_point)
{
	EndPosints[0] = start_point.x;
	EndPosints[1] = start_point.y;
	EndPosints[2] = end_point.x;
	EndPosints[3] = end_point.y;
}