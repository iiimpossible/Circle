#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
 
namespace CircleEngine 
{	 
#define CeLoolAt(eye,center,up) glm::lookAt(eye,center,up)
#define CeTranslate(model,position)glm::translate(model, position)
#define CeRotate(model,radians,axis)glm::rotate(model, radians, axis)
#define CeScale(model,size)glm::scale(model,size)
#define CeRadians(degree)glm::radians(degree)
#define CePerspective(zoom,length_width_ratio,near,far)glm::perspective(glm::radians(zoom), length_width_ratio, near, far)
#define CeOrtho(left,right,bottom,top)glm::ortho(left,right,bottom,top)
#define CeDot(x,y)glm::dot(x,y)
#define CeClamp(target,minval,maxval)glm::clamp(target,minval,maxval)
#define CeCross(left,right)glm::cross(left,right)
#define CeNormalize(vector)glm::normalize(vector)
#define CeLength(vector)glm::length(vector)
#define CeInverse(mat)glm::inverse(mat)  //Ãû³Æ³åÍ»
#define CeTraspose(mat)glm::transpose(mat)
#define CeViewMat2D()
	
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat2 = glm::mat2;
	using Mat3 = glm::mat3;
	using Mat4 = glm::mat4;	  	

	
}

 