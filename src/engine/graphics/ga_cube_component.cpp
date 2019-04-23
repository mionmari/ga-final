/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_cube_component.h"
#include "ga_material.h"

#include "entity/ga_entity.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <cassert>

static const GLfloat k_cube_vertices[] = {
	// Front
	-1.0, -1.0,  1.0,
	1.0, -1.0,  1.0,
	1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	// Top
	-1.0,  1.0,  1.0,
	1.0,  1.0,  1.0,
	1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0,
	// Back
	1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0,  1.0, -1.0,
	1.0,  1.0, -1.0,
	// Bottom
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0, -1.0,  1.0,
	-1.0, -1.0,  1.0,
	// Left
	-1.0, -1.0, -1.0,
	-1.0, -1.0,  1.0,
	-1.0,  1.0,  1.0,
	-1.0,  1.0, -1.0,
	// Right
	1.0, -1.0,  1.0,
	1.0, -1.0, -1.0,
	1.0,  1.0, -1.0,
	1.0,  1.0,  1.0,
};

static const GLfloat k_cube_texcoords[] = {
	// Front
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
	// Top
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
	// Back
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
	// Bottom
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
	// Left
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
	// Right
	0.0, 1.0,
	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
};

static const GLfloat k_cube_normals[] = {
	// Front
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	// Top
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	// Back
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	// Bottom
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	// Left
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	// Right
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
};

static const GLushort k_cube_indices[] = {
	// Front
	0,  1,  2,
	2,  3,  0,
	// Top
	4,  5,  6,
	6,  7,  4,
	// Back
	8,  9, 10,
	10, 11,  8,
	// Bottom
	12, 13, 14,
	14, 15, 12,
	// Left
	16, 17, 18,
	18, 19, 16,
	// Right
	20, 21, 22,
	22, 23, 20,
};

GLuint vbo[4];

void ga_cube_component::create_vertex_array()
{
	// TODO: Homework 3 - Create the vertex array object _vao and any 
	// buffers needed to hold the vertex data provided above.
	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	/* Allocate and assign  Vertex Buffer Objects to our handle */
	glGenBuffers(4, vbo);

	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Copy the vertex data  to our buffer */
	glBufferData(GL_ARRAY_BUFFER, sizeof(k_cube_vertices), k_cube_vertices, GL_STATIC_DRAW);

	/* Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);

	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	/* Bind our second VBO as being the active buffer and storing vertex attributes (uv's) */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	assert(glGetError() == GL_NO_ERROR);

	/* Copy the color data from UVs to our buffer */
	glBufferData(GL_ARRAY_BUFFER, sizeof(k_cube_texcoords), k_cube_texcoords, GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	/* Specify that our uv data is going into attribute index 1, and contains two floats per vertex */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);

	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	assert(glGetError() == GL_NO_ERROR);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(k_cube_indices), k_cube_indices, GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);
	
	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NO_ERROR);

	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	assert(glGetError() == GL_NO_ERROR);
	glBufferData(GL_ARRAY_BUFFER, sizeof(k_cube_normals), k_cube_normals, GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);
	glEnableVertexAttribArray(3);
	assert(glGetError() == GL_NO_ERROR);
	
	
}

void ga_cube_component::destroy_vertex_array()
{
	// TODO: Homework 3 - Do any necessary cleanup here.
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &_vao);

}

ga_cube_component::ga_cube_component(ga_entity* ent, const char* texture_file) : ga_component(ent)
{
	_material = new ga_unlit_texture_material(texture_file);
	_material->init();

	_index_count = uint32_t(sizeof(k_cube_indices) / sizeof(*k_cube_indices));

	create_vertex_array();
}

ga_cube_component::~ga_cube_component()
{
	destroy_vertex_array();

	delete _material;
}

void ga_cube_component::update(ga_frame_params* params)
{
	const float dt = std::chrono::duration_cast<std::chrono::duration<float>>(params->_delta_time).count();

	// Rotate the cube 60 degrees per second around the y-axis

	ga_quatf axis_angle;
	axis_angle.make_axis_angle(ga_vec3f::y_vector(), ga_degrees_to_radians(60.0f) * dt);
	get_entity()->rotate(axis_angle);

	// Prepare the draw calls and add it to the list
	ga_static_drawcall draw;
	draw._name = "ga_cube_component";
	draw._vao = _vao;
	draw._index_count = _index_count;
	draw._transform = get_entity()->get_transform();
	draw._draw_mode = GL_TRIANGLES;
	draw._material = _material;

	while (params->_static_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
	params->_static_drawcalls.push_back(draw);
	params->_static_drawcall_lock.clear(std::memory_order_release);
}
