#version 400

in vec3 o_normal;
in vec3 o_color;
in vec2 o_uv;

uniform sampler2D u_texture;

const vec3 light_dir = normalize(vec3(0,0,1));
const vec3 ambient = vec3(0.25, 0.25, 0.25);
const vec3 diffuse = vec3(1,1,1);
const vec3 specular = vec3(0.5, 0.5, 0.5);

out vec4 finalColor;

void main(void)
{

	vec3 to_light = -light_dir;
	
	vec3 diffuse_color = o_color * max(dot(to_light, o_normal), 0);
	vec3 lit_color = ambient + diffuse_color;
	
	vec4 tex_color = texture(u_texture,o_uv);
	
	finalColor = vec4(lit_color,1.0);
	
}
