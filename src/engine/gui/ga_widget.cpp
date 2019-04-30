/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_widget.h"

#include "framework/ga_drawcall.h"
#include "framework/ga_frame_params.h"

const ga_vec3f ga_widget::k_button_color = { 0.5f, 0.5f, 1.0f };
const ga_vec3f ga_widget::k_button_hover_color = { 0.75f, 0.75f, 1.0f };
const ga_vec3f ga_widget::k_button_press_color = { 0.25f, 0.25f, 1.0f };
const ga_vec3f ga_widget::k_text_color = { 1.0f, 1.0f, 1.0f };
const float ga_widget::k_button_offset = 4.0f;
const float ga_widget::k_checkbox_offset = 30.0f;

void ga_widget::draw_outline(ga_frame_params* params, const ga_vec2f& min, const ga_vec2f& max, const ga_vec3f& color, float offset)
{
	// TODO: Homework 4
	// Check the README
	ga_dynamic_drawcall drawcall;

	drawcall._positions.push_back({ min.x, min.y, 0.0f });
	drawcall._positions.push_back({ max.x, min.y, 0.0f });
	drawcall._positions.push_back({ max.x, max.y, 0.0f });
	drawcall._positions.push_back({ min.x, max.y, 0.0f });

	drawcall._indices.push_back(0);
	drawcall._indices.push_back(1);
	drawcall._indices.push_back(1);
	drawcall._indices.push_back(2);
	drawcall._indices.push_back(2);
	drawcall._indices.push_back(3);
	drawcall._indices.push_back(3);
	drawcall._indices.push_back(0);

	drawcall._color = color;
	drawcall._draw_mode = GL_LINES;
	drawcall._transform.make_identity();
	drawcall._material = nullptr;

	while (params->_gui_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
	params->_gui_drawcalls.push_back(drawcall);
	params->_gui_drawcall_lock.clear(std::memory_order_release);
}

void ga_widget::draw_check(ga_frame_params* params, const ga_vec2f& min, const ga_vec2f& max, const ga_vec3f& color)
{
	// TODO: Homework 4
	// It's like a box, with a smaller number of indices in a different order

	ga_dynamic_drawcall drawcall;

	drawcall._positions.push_back({ min.x, min.y, 0.0f });
	drawcall._positions.push_back({ max.x, min.y, 0.0f });
	drawcall._positions.push_back({ max.x, max.y, 0.0f });
	drawcall._positions.push_back({ min.x, max.y, 0.0f });

	// Create 'X' formation
	drawcall._indices.push_back(0);
	drawcall._indices.push_back(2);
	drawcall._indices.push_back(1);
	drawcall._indices.push_back(3);

	drawcall._color = color;
	drawcall._draw_mode = GL_LINES;
	drawcall._transform.make_identity();
	drawcall._material = nullptr;

	while (params->_gui_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
	params->_gui_drawcalls.push_back(drawcall);
	params->_gui_drawcall_lock.clear(std::memory_order_release);
}

void ga_widget::draw_fill(ga_frame_params* params, const ga_vec2f& min, const ga_vec2f& max, const ga_vec3f& color)
{
	// TODO: Homework 4
	// Draw with triangles instead of lines

	ga_dynamic_drawcall drawcall;

	drawcall._positions.push_back({ min.x, min.y, 0.0f });
	drawcall._positions.push_back({ max.x, min.y, 0.0f });
	drawcall._positions.push_back({ max.x, max.y, 0.0f });
	drawcall._positions.push_back({ min.x, max.y, 0.0f });

	drawcall._indices.push_back(0);
	drawcall._indices.push_back(1);
	drawcall._indices.push_back(2);
	drawcall._indices.push_back(0);
	drawcall._indices.push_back(3);
	drawcall._indices.push_back(2);

	drawcall._color = color;
	drawcall._draw_mode = GL_TRIANGLES;
	drawcall._transform.make_identity();
	drawcall._material = nullptr;

	while (params->_gui_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
	params->_gui_drawcalls.push_back(drawcall);
	params->_gui_drawcall_lock.clear(std::memory_order_release);
}

// Detects if mouse is within range of this widget
bool ga_widget::mouse_in_range(const ga_frame_params* params, const ga_vec2f& min, const ga_vec2f& max) const
{
	return params->_mouse_x >= min.x - 10 &&
		params->_mouse_y >= min.y - 20 &&
		params->_mouse_x <= max.x - 10 &&
		params->_mouse_y <= max.y - 20;
}