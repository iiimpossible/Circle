#include "Math/Math.h"
#include "Tool/CeStructs.h"
#include "Tool/CeEnums.h"

namespace CircleEngine
{
	typedef unsigned ShaderID;//着色器句柄
	typedef unsigned Texture2DID;//纹理句柄
	typedef unsigned VAOID;//顶点数组对象句柄
	typedef unsigned VBOID;//顶点缓冲对象句柄
	typedef unsigned EBOHd;//索引缓冲对象句柄
	typedef unsigned TBOID;//纹理缓冲对象句柄
	typedef unsigned FBOID;//帧缓冲对象句柄
	typedef unsigned RBOID;//渲染缓冲对象句柄

	extern float ViewportWidth;
	extern float ViewportHeight;
#define VIEWPORTWIDTH 800.0f
#define VIEWPORTHEIGHT 600.0f

//100像素 以100像素为 正视投影的单位长度
#define UNITLENGTH 100.0f
	/**UnitLength 是2D世界的单位长度 ，为100像素。如果是3D的话，就得改为1*/
	extern const float UnitLength;
	extern const float UnitLengthInverse;


#ifdef WORLD3D

#endif // WORLD3D

#ifdef WORLD2D

#endif // WORLD2D


	 




}




 