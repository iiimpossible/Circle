#version 330 core
out vec4 oFragColor;

in vec2 TexCoords;

//正常场景渲染结果
uniform sampler2D iScene;
//高亮光模糊结果
uniform sampler2D iBloom;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	const float brightness = 0.3;
	vec3 scene = texture(iScene, TexCoords).rgb;
	vec3 bloom = texture(iBloom, TexCoords).rgb;

	//人物模型纹理应该是已经进行过γ矫正了，所以只对高亮纹理进行γ矫正
	bloom = pow(bloom, vec3(1.0 / gamma));
	scene += bloom; // 颜色相加混合
	
	//色调映射，关键exp是以自然常数为底的指数函数
	//把scene取反，而scene属于[0,∞],从而-scene[-∞.0,0]
	//从而exp(-scene)属于[-∞,1]，然后用1.0减去前面，从而映射到[0,1]
	//brightness参数调整计算后的颜色失真程度
	vec3 result = vec3(1.0) - exp(-scene * brightness);	
	oFragColor = vec4(result/ (brightness*0.9), 1.0);
}