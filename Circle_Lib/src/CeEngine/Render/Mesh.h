#pragma once
#include <string>
#include <vector>
#include <Celib_x64/src/CeCommon/Common.h>
using CircleEngine::Vec3;
using CircleEngine::Vec2;
using std::string;
using std::vector;


//struct TexCoord
//{
//	float u;
//	float v;
//};
//
//struct Normal
//{
//	float x;
//	float y;
//	float z;
//};

namespace CircleEngine
{
	struct Texture
	{
		unsigned id;
		string type;
		string path;
	};

	struct Vertex
	{
		Vec3 Position;
		Vec3 Normal;
		Vec2 TexCoord;
	};

	struct Mesh
	{
		vector<Vertex> Vertices;
		vector<unsigned> Indices;
		vector<Texture> Textures;

		unsigned VAO;
		unsigned VBO;
		unsigned EBO;
	};
}

