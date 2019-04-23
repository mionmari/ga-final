#version 400

uniform vec3 u_diffuse;
uniform bool u_lighting_enabled;

in vec2 fragmentUV;
in vec3 fragmentNormal;

const vec3 light_dir = normalize(vec3(1,-.5,1));
const vec3 light_ambient = vec3(0.25, 0.25, 0.25);
const vec3 light_diffuse = vec3(1,1,1);
const vec3 light_specular = vec3(0.5, 0.5, 0.5);

out vec4 finalColor;

void main()
{
	if (u_lighting_enabled)
	{
		vec3 to_light = -light_dir;
		vec3 diffuse_lit = u_diffuse * max(dot(to_light, fragmentNormal), 0);
		vec3 lit_color = light_ambient + diffuse_lit;
		finalColor = vec4(lit_color,1.0);
	}
	else
	{
		finalColor = vec4(u_diffuse,1.0);
	}
}