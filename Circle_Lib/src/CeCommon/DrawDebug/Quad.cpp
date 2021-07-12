#include "Quad.h"
using CircleEngine::rectangle_fragment_shader;
using CircleEngine::rectangle_vertex_shader;
using CircleEngine::CreateVAO;
using CircleEngine::CreateVBO;
using CircleEngine::CreateVBOEmpty;
using CircleEngine::ShaderCompile;

Quad::Quad(Vec2 position, Vec2 size, Vec3 color) :
	Position(position), Size(size), Color(color)

{
	CaculateVertices(position, size);
	VAO = CreateVAO();
	VBO = CreateVBOEmpty(this->VAO, 16, 2, 2);
	ShaderProgramID = ShaderCompile(CircleEngine::rectangle_vertex_shader, CircleEngine::rectangle_fragment_shader, nullptr);
}

Quad::~Quad()
{
}

float * Quad::GetVertices()
{
	return Vertices;
}
/**
 * @brief
 *+！！！！！！！！！！！！！！！！！！+
 *|                  |
 *|                  |
 *|                  |
 *+！！！！！！！！！！！！！！！！！！+
 */
void Quad::CaculateVertices(Vec2 position, Vec2 size)
{
	//�澡�
	Vertices[0] = position.x;//0
	Vertices[1] = position.y;//0

	Vertices[2] = position.x + size.x;//0.5
	Vertices[3] = position.y;//0

	Vertices[4] = position.x + size.x;//0.5
	Vertices[5] = position.y;//0

	Vertices[6] = position.x + size.x;//0.5
	Vertices[7] = position.y + size.y;//0.5

	Vertices[8] = position.x + size.x;//0.5
	Vertices[9] = position.y + size.y;//0.5

	Vertices[10] = position.x;//0
	Vertices[11] = position.y + size.y;//0.5

	Vertices[12] = position.x;//0
	Vertices[13] = position.y + size.y;//0.5

	Vertices[14] = position.x;//0
	Vertices[15] = position.y;//0
}
