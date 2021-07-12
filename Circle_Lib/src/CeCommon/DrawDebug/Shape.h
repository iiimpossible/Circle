#pragma once

class Shape
{
public:
	Shape();
	virtual ~Shape();
	unsigned GetVAO()const;
	unsigned GetVBO()const;
	unsigned GetShaderID()const;
	virtual void CaculateVertices();
protected:
	unsigned VAO;
	unsigned VBO;
	unsigned ShaderProgramID;
	
};

