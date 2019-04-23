#version 400

uniform mat4 u_mvp;

layout(location = 0) in vec3 in_vertex;

void main(void)
{
	gl_Position = u_mvp * vec4(in_vertex, 1.0);
}