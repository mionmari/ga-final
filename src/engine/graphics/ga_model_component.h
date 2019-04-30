#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "entity/ga_component.h"
#include <assimp/scene.h>           // Output data structure

#include <cstdint>
#include <vector>
/*
** Renderable 3d model loaded from a file
*/

struct ga_maps 
{
	const char* albedo_map;
	const char* normal_map;
	const char* metallic_map;
	const char* roughness_map;
	const char* ao_map;
};

class ga_mesh
{
public:
	ga_mesh();
	~ga_mesh();

	void create_from_aiMesh(aiMesh* mesh, const aiScene* scene, const struct ga_maps* maps);
	void assemble_drawcall(ga_static_drawcall& draw);

private:
	class ga_material* _material;

	uint32_t _index_count;
	uint32_t _vao;
	GLuint _vbo[5];
	std::vector<GLushort> _index_array;
	std::vector<ga_vec3f> _vertex_array;
	std::vector<ga_vec3f> _normals;
	std::vector<ga_vec2f> _texcoords;
	std::vector<ga_vec3f> _tangents;
};

class ga_model_component : public ga_component
{
public:
	ga_model_component(class ga_entity* ent, const char* model_file, const struct ga_maps* maps);
	virtual ~ga_model_component();

	virtual void update(struct ga_frame_params* params) override;
	void process_node_recursive(aiNode* node, const aiScene* scene, const struct ga_maps* maps);

private:
	std::vector<ga_mesh*> _meshes;
	const aiScene *_scene;
};


