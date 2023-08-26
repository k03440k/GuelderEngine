#version 460

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(push_constant) uniform constants
{
	mat4 model;
} ObjectData;

layout(location = 0) out vec3 color;

void main()
{
	gl_Position = /*ObjectData.model **/ vec4(inPosition, 0.0f, 1.0f);
	color = inColor;
}