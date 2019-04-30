#version 400

layout(location = 0) in vec3 in_vertex;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 in_normal;
layout (location = 4) in vec3 in_tangent;

out vec2 o_uv;
out vec3 o_normal;
out vec3 o_tangent;
out vec3 o_worldPos;

uniform mat4 u_mvp;
uniform mat4 u_world;

void main()
{
	o_uv = in_texcoord;
	o_normal = (u_mvp * vec4(in_normal, 0.0)).xyz;
	o_tangent = (u_mvp * vec4(in_tangent, 0.0)).xyz;
	vec4 pos = u_mvp * vec4(in_vertex, 1.0);
	o_worldPos = pos.xyz;
	gl_Position = pos;
}