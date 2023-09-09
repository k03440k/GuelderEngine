#version 460

layout(location = 0) in vec3 color;
layout(location = 0) out vec4 fragColor;

layout(push_constant) uniform Push
{
	vec2 pos;
	vec3 color;
} push;

void main()
{
	fragColor = vec4(push.color + color, 1.0f);
}