/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_model_component.h"
#include "ga_material.h"

#include "entity/ga_entity.h"

/* assimp include files. These three are usually needed. */

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <cassert>

ga_model_component::ga_model_component(ga_entity* ent, const char* model_file, const struct ga_maps* maps) : ga_component(ent)
{
	extern char g_root_path[256];
	Assimp::Importer importer;
	std::string model_path = g_root_path;
	model_path += model_file;

	_scene = importer.ReadFile(model_path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	
	if (!_scene)
	{
		std::cout << "error: couldn't load obj\n";
	}
	else
	{
		// process the scene
		process_node_recursive(_scene->mRootNode, _scene, maps);
	}
}

ga_model_component::~ga_model_component()
{
	
}

void ga_model_component::process_node_recursive(aiNode* node, const aiScene* scene, const struct ga_maps* maps)
{
	// process my meshes
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		ga_mesh* mesh = new ga_mesh();
		mesh->create_from_aiMesh(_scene->mMeshes[node->mMeshes[i]], _scene, maps);
		_meshes.push_back(mesh);
	}
	// process my children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		process_node_recursive(node->mChildren[i], scene, maps);
	}
}

void ga_model_component::update(ga_frame_params* params)
{
	const float dt = std::chrono::duration_cast<std::chrono::duration<float>>(params->_delta_time).count();
	get_entity()->rotate({ 0,60.0f*dt,0 });

	for (ga_mesh* m : _meshes)
	{
		ga_static_drawcall draw;
		draw._name = "model";
		m->assemble_drawcall(draw);
		draw._transform = get_entity()->get_transform();

		while (params->_static_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
		params->_static_drawcalls.push_back(draw);
		params->_static_drawcall_lock.clear(std::memory_order_release);
	}
}

ga_mesh::ga_mesh()
{
	_index_count = 0;
	_material = nullptr;
	_vao = 0;
}
ga_mesh::~ga_mesh()
{
	glDeleteBuffers(5, _vbo);
	glDeleteVertexArrays(1, &_vao);

}

void ga_mesh::create_from_aiMesh(aiMesh* mesh, const aiScene* scene, const struct ga_maps* maps)
{
	GLenum err;
	
	// request vertex array object and vertex buffer objects
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(5, _vbo);

	// TODO: Homework 4
	// load vertex positions, indices, uv's and normals from importer mesh into our data structure

	/*ga_lit_material* mat = new ga_lit_material();
	mat->init();*/


	ga_pbr_material* mat = new ga_pbr_material
	(	
		maps->albedo_map,
		maps->normal_map, 
		maps->metallic_map, 
		maps->roughness_map, 
		maps->ao_map
	);
	mat->init();
	_material = mat;


	// Load vertex positions
	for (int i = 0; i < mesh->mNumVertices; ++i) 
	{
		ga_vec3f vert = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		_vertex_array.push_back(vert);
	}


	// Load indices
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		_index_array.push_back(mesh->mFaces[i].mIndices[0]);
		_index_array.push_back(mesh->mFaces[i].mIndices[1]);
		_index_array.push_back(mesh->mFaces[i].mIndices[2]);
	}

	// Load texture uvs
	if (mesh->HasTextureCoords(0)) 
	{
		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			ga_vec2f tex = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			_texcoords.push_back(tex);
		}
	}

	// Load normals
	for (int i = 0; i < mesh->mNumVertices; ++i) 
	{
		ga_vec3f norm = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		_normals.push_back(norm);
	}

	// Compute tangents
	_tangents.resize(mesh->mNumVertices, ga_vec3f::zero_vector());
	for (unsigned int i = 0; i < _index_array.size(); i += 3) {
		GLushort index0 = _index_array[i];
		GLushort index1 = _index_array[i + 1];
		GLushort index2 = _index_array[i + 2];

		ga_vec3f v0 = _vertex_array[index0];
		ga_vec3f v1 = _vertex_array[index1];
		ga_vec3f v2 = _vertex_array[index2];

		ga_vec3f edge1 = v1 - v0;
		ga_vec3f edge2 = v2 - v0;

		float deltaU1 = _texcoords[index1].x - _texcoords[index0].x;
		float deltaV1 = _texcoords[index1].y - _texcoords[index0].y;
		float deltaU2 = _texcoords[index2].x - _texcoords[index0].x;
		float deltaV2 = _texcoords[index2].y - _texcoords[index0].y;

		float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

		ga_vec3f tangent, bitangent;

		tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
		tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
		tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

		bitangent.x = f * (-deltaU2 * edge1.x - deltaU1 * edge2.x);
		bitangent.y = f * (-deltaU2 * edge1.y - deltaU1 * edge2.y);
		bitangent.z = f * (-deltaU2 * edge1.z - deltaU1 * edge2.z);

		_tangents[index0] += tangent;
		_tangents[index1] += tangent;
		_tangents[index2] += tangent;
	}

	for (unsigned int i = 0; i < _tangents.size(); i++) {
		_tangents[i].normalize();
	}

	// TODO: Homework 4 
	// set up vertex and element array buffers for positions, indices, uv's and normals

	// Set up buffer for positions
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	assert(glGetError() == GL_NO_ERROR);
	glBufferData(GL_ARRAY_BUFFER, _vertex_array.size() * sizeof(ga_vec3f), &_vertex_array.front(), GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);
	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	// Set up buffer for indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
	assert(glGetError() == GL_NO_ERROR);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _index_array.size() * sizeof(GLushort), &_index_array.front(), GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	_index_count = _index_array.size();

	glVertexAttribPointer(1, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);
	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);


	// Set up buffer for texcoords uvs
	if (mesh->HasTextureCoords(0))
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
		assert(glGetError() == GL_NO_ERROR);
		glBufferData(GL_ARRAY_BUFFER, _texcoords.size() * sizeof(ga_vec2f), &_texcoords.front(), GL_STATIC_DRAW);
		assert(glGetError() == GL_NO_ERROR);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		assert(glGetError() == GL_NO_ERROR);
		glEnableVertexAttribArray(2);
		assert(glGetError() == GL_NO_ERROR);
	}

	// Set up buffer for normals
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[3]);
	assert(glGetError() == GL_NO_ERROR);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(ga_vec3f), &_normals.front(), GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);
	glEnableVertexAttribArray(3);
	assert(glGetError() == GL_NO_ERROR);

	// Set up buffer for tangents
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[4]);
	assert(glGetError() == GL_NO_ERROR);
	glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(ga_vec3f), &_tangents.front(), GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);
	glEnableVertexAttribArray(4);
	assert(glGetError() == GL_NO_ERROR);

	// unbind vertex array
	glBindVertexArray(0);

	err = glGetError();
	assert(err == GL_NO_ERROR);
}

void ga_mesh::assemble_drawcall(ga_static_drawcall& draw)
{
	draw._vao = _vao;
	draw._index_count = _index_count;
	draw._draw_mode = GL_TRIANGLES;
	draw._material = _material;
}



