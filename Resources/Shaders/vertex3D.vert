#version 460

layout(location = 0) in vec3 inPosition;//3D
layout(location = 1) in vec3 inColor;

layout(push_constant) uniform Push
{
	mat4 transform;//3D
	//vec2 offset;
	vec3 color;
} push;

layout(location = 0) out vec3 inFragColor;

void main()
{
	gl_Position = push.transform * vec4(inPosition, 1.0f);//3D
	inFragColor = inColor;
}