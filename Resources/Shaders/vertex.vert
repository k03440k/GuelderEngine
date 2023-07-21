#version 460

vec2 positions[3] = vec2[](
	vec2(0.0f, -0.05f),
	vec2(0.05f, 0.05f),
	vec2(-0.05f, 0.05f)
);

vec3 colors[3] = vec3[](
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f)
);

layout(push_constant) uniform constants
{
	mat4 model;
} ObjectData;

layout(location = 0) out vec3 color;

void main()
{
	gl_Position = 	ObjectData.model * vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
	color = colors[gl_VertexIndex];
}