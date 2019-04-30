#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "math/ga_mat4f.h"
#include "entity/ga_component.h"

/*
** Represents the a camera used to navigate the simulation world.
*/
class ga_camera : public ga_component
{
public:
	// The camera assumes an initial direction along the positive z axis, and
	// up vector along the positive y.
	ga_camera(class ga_entity* ent);
	~ga_camera();

	void update(struct ga_frame_params* params);
};
