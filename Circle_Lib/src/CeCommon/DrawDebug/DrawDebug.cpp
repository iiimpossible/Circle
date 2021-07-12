#include <GL/glew.h>
#include "DrawDebug.h"
#include "../Math/Math.h"
using CircleEngine::ReWriteVBO;
using CircleEngine::Mat4;
using CircleEngine::ViewportWidth;
using CircleEngine::ViewportHeight;


//line DebugDraw::Line;
//Quad DebugDraw::Quad;
DrawDebug::DrawDebug()
{

}

DrawDebug::~DrawDebug()
{
}

void DrawDebug::DrawLine(Vec3 start_point, Vec3 end_point, Vec3 color)
{
	static line Line;
	
	Line.CaculateVertices(start_point, end_point);

	glUseProgram(Line.GetShaderID());

	glUniform3f(glGetUniformLocation(Line.GetShaderID(), "color"), color.x, color.y, color.z);

	Mat4 projection;
	projection = CC_Ortho(0.0f, ViewportWidth, ViewportHeight, 0.0f);

	glUniformMatrix4fv(glGetUniformLocation(Line.GetShaderID(), "projection"), 1, GL_FALSE, &projection[0][0]);

	ReWriteVBO(Line.GetVBO(), Line.GetIndices(), 4);

	glBindVertexArray(Line.GetVAO());

	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);
}

void DrawDebug::DrawQuad(Vec2 position, Vec2 size, Vec3 color)
{
	static Quad Quad;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Quad.CaculateVertices(position, size);

	glUseProgram(Quad.GetShaderID());

	glUniform3f(glGetUniformLocation(Quad.GetShaderID(), "color"), color.x, color.y, color.z);

	glm::mat4 projection;
	projection = CC_Ortho(0.0f, ViewportWidth, ViewportHeight, 0.0f);

	glUniformMatrix4fv(glGetUniformLocation(Quad.GetShaderID(), "projection"), 1, GL_FALSE, &projection[0][0]);

	ReWriteVBO(Quad.GetVBO(), Quad.GetVertices(), 16);

	glBindVertexArray(Quad.GetVAO());

	glDrawArrays(GL_LINES, 0, 8);

	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
}
