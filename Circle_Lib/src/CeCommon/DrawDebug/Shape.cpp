#include "Shape.h"
 
#include <gl/glew.h>
Shape::Shape()
{
}

Shape::~Shape()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

unsigned Shape::GetVAO() const
{
	return VAO;
}

unsigned Shape::GetVBO() const
{
	return VBO;
}

unsigned Shape::GetShaderID() const
{
	return ShaderProgramID;
}

void Shape::CaculateVertices()
{
}

