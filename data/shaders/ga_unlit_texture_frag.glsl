#version 400

uniform sampler2D u_texture;
in vec2 fragmentUV;

// TODO: Homework 3 - Add any other necessary uniforms or input/output variables here

void main()
{
	// TODO: Homework 3 
	gl_FragColor = vec4(1.,1.,0.,1.);
	gl_FragColor = texture(u_texture, fragmentUV);
}