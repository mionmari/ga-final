#version 400
layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexNormal;

out vec2 fragmentUV;
out vec3 fragmentNormal;

uniform mat4 u_mvp;

void main()
{
	fragmentUV = vertexUV;
	fragmentNormal = (u_mvp * vec4(vertexNormal, 0.0)).xyz;
	gl_Position = u_mvp * vec4(vertexPosition, 1.0);

}