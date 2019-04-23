#include "graphics/ga_mesh.h"
#include "graphics/ga_node.h"
#include "entity/ga_entity.h"
#include "math/ga_mat4f.h"
#include "graphics/ga_material.h"
#include <vector>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <cassert>
#include <string>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/scene.h>           // Output data structure

void ai_mat4_to_ga_mat4(aiMatrix4x4& in_mat, ga_mat4f& out_mat)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			out_mat.data[r][c] = in_mat[r][c];
		}
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
	glDeleteBuffers(2, _vbo);
	glDeleteVertexArrays(1, &_vao);

}

void ga_mesh::create_from_aiMesh(aiMesh* mesh, const aiScene* scene)
{
	_vertex_format = 1;
	_name = mesh->mName.C_Str();
	// set vertex format bitmask
	if (mesh->HasTextureCoords(0))
	{
		_vertex_format |= k_vertex_attribute_uv;
	}
	if (mesh->HasNormals())
	{
		_vertex_format |= k_vertex_attribute_normal;
	}
	if (mesh->HasVertexColors(0))
	{
		_vertex_format |= k_vertex_attribute_color;
	}
	if (mesh->HasBones())
	{
		_vertex_format |= k_vertex_attribute_weight;
	}

	// load vertex positions 
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		ga_vertex vertex = {};
		vertex._position.x = mesh->mVertices[i].x;
		vertex._position.y = mesh->mVertices[i].y;
		vertex._position.z = mesh->mVertices[i].z;

		if (_vertex_format & k_vertex_attribute_uv)
		{
			vertex._uv.x = mesh->mTextureCoords[0][i].x;
			vertex._uv.y = mesh->mTextureCoords[0][i].y;
		}
		if (_vertex_format & k_vertex_attribute_normal)
		{
			vertex._normal.x = mesh->mNormals[i].x;
			vertex._normal.y = mesh->mNormals[i].y;
			vertex._normal.z = mesh->mNormals[i].z;
		}
		
		_vertex_array.push_back(vertex);
	}
	
	// assign joint influences and weights
	if (_vertex_format & k_vertex_attribute_weight)
	{
		for (int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];
			ga_joint* joint = new ga_joint();

			joint->_name = bone->mName.C_Str();

			ai_mat4_to_ga_mat4(bone->mOffsetMatrix, joint->_inv_bind_matrix);
			joint->_inv_bind_matrix.transpose();
			joint->_world_matrix = joint->_inv_bind_matrix;
			joint->_world_matrix.invert();

			joint->_skin_matrix = joint->_inv_bind_matrix * joint->_world_matrix;				// skin = inv_bind * world
			_skeleton._joints.push_back(joint);

			uint32_t joint_index = _skeleton._joints.size() - 1;

			for (int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight vertex_weight = bone->mWeights[w];

				int vert_idx = bone->mWeights[w].mVertexId;
				float weight = bone->mWeights[w].mWeight;

				ga_vertex* vert = &_vertex_array[vert_idx];

				vert->_joints[vert->_num_influences] = joint_index;
				
				vert->_weights[vert->_num_influences] = weight;
				vert->_num_influences++;
			}
		}
	}

	// load indices
	for (int f = 0; f < mesh->mNumFaces; f++)
	{
		aiFace face = mesh->mFaces[f];
		_index_array.push_back(face.mIndices[0]); 
		_index_array.push_back(face.mIndices[1]);
		_index_array.push_back(face.mIndices[2]);
		if (face.mNumIndices > 3)
		{
			std::cout << "Face with more than 3 verts: mNumIndices=" << face.mNumIndices << std::endl;
		}
	}

	// load material 
	ga_animated_material* mat = new ga_animated_material(&_skeleton);
	mat->init();

	// get color 
	aiColor3D ai_diffuse;
	aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
	ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_diffuse);
	//mat->set_diffuse (ai_diffuse.r, ai_diffuse.g, ai_diffuse.b);

	_material = mat;

}

void ga_mesh::make_buffers()
{
	GLenum err;

	// Create vertex array objet and vertex buffers 
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(2, _vbo);

	GLsizei vertex_size = sizeof(_vertex_array[0]);

	// bind buffer data
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, _vertex_array.size() * vertex_size, _vertex_array.data(), GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, 0);
	glEnableVertexAttribArray(0);

	// normal
	if (_vertex_format & k_vertex_attribute_normal)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)12);
		glEnableVertexAttribArray(1);
	}
	// color
	if (_vertex_format & k_vertex_attribute_color)
	{
		// Color values for each vertex.
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)24);
		glEnableVertexAttribArray(2);
	}
	if (_vertex_format & k_vertex_attribute_uv)
	{
		// UV coordinates of each vertex.
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)36);
		glEnableVertexAttribArray(3);
	}
	if (_vertex_format & k_vertex_attribute_weight)
	{
		// Joint indices that influence each vertex.  Note that these are unsigned integers.
		glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, vertex_size, (GLvoid*)44);
		glEnableVertexAttribArray(4);

		// Joint weights that influence each vertex.
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)60);
		glEnableVertexAttribArray(5);
	}

	// bind buffer for element index array
	_index_count = _index_array.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _index_count * sizeof(GLushort), _index_array.data(), GL_STATIC_DRAW);

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

void ga_mesh::update(ga_frame_params* params)
{
	// update joints:
	// get the world matrix from the transform node
	// multiply by inverse skinning matrix
	// store in world matrix

	for (ga_joint* joint: _skeleton._joints)
	{
		if (joint->_node_ptr != nullptr)
		{
			joint->_world_matrix = joint->_node_ptr->_world_transform;
			joint->_skin_matrix =  joint->_inv_bind_matrix * joint->_world_matrix;
		}
	}
}