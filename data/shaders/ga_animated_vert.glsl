#version 400

uniform mat4 u_mvp;

//****

// TODO: Homework 6.
// First, create a mat4 array to hold the skinning matrices of the skeleton.
// Refer to ga_animated_material::bind for clues on how to define this array.
uniform mat4 u_skin[75];
uniform bool u_hasweights;

// Next, create the additional input variables for vertex attributes of the bar model.


//****
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 3) in vec2 in_texcoord;
layout (location = 4) in ivec4 in_jointindices;
layout (location = 5) in vec4 in_jointweights;


//****

out vec3 o_normal;
out vec3 o_color;
out vec2 o_uv;

void main(void)
{

	// TODO: Homework 6.
	// Implement skinning math.
	// Each vertex must be multiplied by the skinning matrices of the joints
	// that influence it, and each result weighted by the influence of each joint.
	// The sum of the transformed, weighted positions is the skinned position of
	// the vertex.  It can then be multiplied by the mvp matrix.
    //****
	
	if(u_hasweights == false)
	{
		gl_Position = u_mvp * vec4(in_vertex,1.0);
		o_normal = (u_mvp * vec4(in_normal, 0.0)).xyz;
		
		o_color = vec3(1.0, 1.0, 1.0);
		o_uv = in_texcoord;
	}
	else
	{
		vec3 skinnedPos =  
		(
			vec4(in_vertex, 1.0) * u_skin[in_jointindices[0]] * in_jointweights[0] +
			vec4(in_vertex, 1.0) * u_skin[in_jointindices[1]] * in_jointweights[1] +
			vec4(in_vertex, 1.0) * u_skin[in_jointindices[2]] * in_jointweights[2] +
			vec4(in_vertex, 1.0) * u_skin[in_jointindices[3]] * in_jointweights[3]  
		).xyz;

		vec3 skinnedNormal = 
			in_normal * (mat3(u_skin[in_jointindices[0]]) * in_jointweights[0]) +
			in_normal * (mat3(u_skin[in_jointindices[1]]) * in_jointweights[1]) +
			in_normal * (mat3(u_skin[in_jointindices[2]]) * in_jointweights[2]) +
			in_normal * (mat3(u_skin[in_jointindices[3]]) * in_jointweights[3]);

		skinnedNormal = normalize(skinnedNormal);

		gl_Position = u_mvp * vec4(skinnedPos, 1.0);
		o_normal = (u_mvp * vec4(skinnedNormal, 0.0)).xyz;

		o_color = vec3(1.0, 1.0, 1.0);
		o_uv = in_texcoord;
	}
	
	//*****
}