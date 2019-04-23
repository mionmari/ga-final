/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_camera.h"
#include "entity/ga_entity.h"
#include "ga_frame_params.h"
#include "math/ga_math.h"
#include <iostream>
ga_camera::ga_camera(ga_entity* ent) : ga_component (ent)
{

}

ga_camera::~ga_camera()
{
}

void ga_camera::update(ga_frame_params* params)
{
	ga_mat4f transform = get_entity()->get_transform();

	const float k_move_speed = 0.1f;
	const float k_rotate_speed = 0.5f;

	// Use WASD to control the position.
	ga_vec3f translation = { 0.0f, 0.0f, 0.0f };

	translation += (params->_button_mask & k_button_d) ? -transform.get_right() : ga_vec3f::zero_vector();
	translation += (params->_button_mask & k_button_a) ? transform.get_right() : ga_vec3f::zero_vector();
	translation += (params->_button_mask & k_button_s) ? -transform.get_forward() : ga_vec3f::zero_vector();
	translation += (params->_button_mask & k_button_w) ? transform.get_forward() : ga_vec3f::zero_vector();
	translation += (params->_button_mask & k_button_q) ? -transform.get_up() : ga_vec3f::zero_vector();
	translation += (params->_button_mask & k_button_e) ? transform.get_up() : ga_vec3f::zero_vector();
	translation.scale(k_move_speed);

	// By using the camera's directional vectors, we've defined the translation in world space.
	ga_mat4f world_translation;
	world_translation.make_translation(translation);
	_transform = _transform * world_translation;

	// Use arrow keys to pitch and rotate.
	float rotation = 0.0f;
	rotation += (params->_button_mask & k_button_left) ? k_rotate_speed : 0.0f;
	rotation += (params->_button_mask & k_button_right) ? -k_rotate_speed : 0.0f;
	float pitch = 0.0f;
	pitch += (params->_button_mask & k_button_up) ? -k_rotate_speed : 0.0f;
	pitch += (params->_button_mask & k_button_down) ? k_rotate_speed : 0.0f;

	rotation = ga_degrees_to_radians(rotation);
	pitch = ga_degrees_to_radians(pitch);

	ga_quatf q1, q2, q3;
	q1.make_axis_angle(ga_vec3f::y_vector(), rotation);
	q2.make_axis_angle(ga_vec3f::x_vector(), pitch);
	q3 = q1 * q2; 
	
	get_entity()->translate(translation);
	get_entity()->rotate(q3);

	ga_vec3f eye = get_entity()->get_transform().get_translation();
	ga_vec3f at = eye + get_entity()->get_transform().get_forward();
	ga_vec3f up = ga_vec3f::y_vector();

	ga_mat4f view;
	view.make_lookat_rh(eye, at, up);

	params->_view = view;
}

void ga_camera::rotate(const ga_quatf& rotation)
{
	ga_mat4f rotation_matrix;
	rotation_matrix.make_rotation(rotation);
	_transform = rotation_matrix * _transform;
}
