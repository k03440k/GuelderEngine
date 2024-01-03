#version 460

layout(location = 0) in vec3 inFragColor;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push
{
	mat4 transform;//3D
	//vec2 offset;
	vec3 color;
} push;

void main()
{
	outColor = vec4(push.color + inFragColor, 1.0f);
}