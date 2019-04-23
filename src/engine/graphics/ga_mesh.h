#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/scene.h>           // Output data structure

#include <vector>
#include <string>

#include "framework/ga_drawcall.h"
#include "graphics/ga_skeleton.h"
#include "framework/ga_frame_params.h"

void ai_mat4_to_ga_mat4(aiMatrix4x4& in_mat, ga_mat4f& out_mat);
/*
* vertex data
*/

struct ga_vertex
{
	static const uint32_t k_max_joint_weights = 4;

	struct ga_vec3f _position;
	struct ga_vec3f _normal;
	struct ga_vec3f _color;
	struct ga_vec2f _uv;

	uint32_t _joints[4] = { 0, 0, 0, 0 };
	float _weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	uint8_t _num_influences = 0;
};

enum ga_vertex_attribute
{
	k_vertex_attribute_normal = 1,
	k_vertex_attribute_color = 2,
	k_vertex_attribute_uv = 4,
	k_vertex_attribute_weight = 8,
};


/*
** Renderable 3d model loaded from a file
*/
class ga_mesh
{
public:
	ga_mesh();
	~ga_mesh();

	void create_from_aiMesh(aiMesh* mesh, const aiScene* scene);
	void make_buffers();
	void assemble_drawcall(ga_static_drawcall& draw);
	void update(ga_frame_params* params);
	std::string _name;
	struct ga_mat4f _transform;
	ga_skeleton _skeleton;
private:
	class ga_material* _material;

	uint32_t _index_count;
	uint32_t _vao;
	GLuint _vbo[2];

	std::vector<struct ga_vertex> _vertex_array;
	std::vector<uint16_t> _index_array;

	uint32_t _vertex_format;
};