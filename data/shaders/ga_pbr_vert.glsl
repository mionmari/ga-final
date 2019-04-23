#version 400
layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexNormal;

out vec2 o_uv;
out vec3 o_normal;
out vec4 o_worldPos;

uniform mat4 u_mvp;


void main()
{
	o_uv = vertexUV;
	o_normal = (u_mvp * vec4(vertexNormal, 0.0)).xyz;
	o_worldPos = u_mvp * vec4(vertexPosition, 1.0);
	gl_Position = o_worldPos;

}