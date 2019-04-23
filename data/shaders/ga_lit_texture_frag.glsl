#version 400

uniform sampler2D u_texture;
in vec2 fragmentUV;
in vec3 fragmentNormal;

const vec3 light_dir = normalize(vec3(1,-.5,1));
const vec3 ambient = vec3(0.25, 0.25, 0.25);
const vec3 diffuse = vec3(1,1,1);
const vec3 specular = vec3(0.5, 0.5, 0.5);

out vec4 finalColor;


void main()
{
	vec3 to_light = -light_dir;
	
	vec3 diffuse_color = diffuse * max(dot(to_light, fragmentNormal), 0);
	vec3 lit_color = ambient + diffuse_color;
	
	vec4 tex_color = texture(u_texture,fragmentUV);
	
	finalColor = vec4(lit_color * vec3(tex_color),1.0);
}