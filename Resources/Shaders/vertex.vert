#version 460

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(push_constant) uniform Push
{
	mat2 transform;
	vec2 offset;
	vec3 color;
} push;

layout(location = 0) out vec3 color;

void main()
{
	gl_Position = vec4(push.offset + push.transform * inPosition, 0.0f, 1.0f);
	color = inColor;
}