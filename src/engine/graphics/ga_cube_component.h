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

#include <cstdint>

/*
** Renderable basic textured cubed.
*/
class ga_cube_component : public ga_component
{
public:
	ga_cube_component(class ga_entity* ent, const char* texture_file);
	virtual ~ga_cube_component();

	virtual void update(struct ga_frame_params* params) override;

private:
	void create_vertex_array();
	void destroy_vertex_array();

	class ga_material* _material;
	
	uint32_t _index_count;

	uint32_t _vao;

	// TODO: Homework 3 - Add any additional members you need here (ie. Handles to vertex arrays and buffers)
};
