/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_checkbox.h"
#include "ga_font.h"

#include "framework/ga_frame_params.h"

ga_checkbox::ga_checkbox(bool state, const char* text, float x, float y, ga_frame_params* params)
{
	// TODO: Homework 4

	// Compute min/max
	_min = { x, y };
	_max = { x + k_checkbox_offset, y + k_checkbox_offset };

	// Draw check if needed
	if (state == true)
		draw_check(params, _min, _max, k_button_color);

	draw_outline(params, _min, _max, k_button_color, k_button_offset); // outline

	if (get_hover(params)) 
	{
		draw_fill(params, _min, _max, ga_vec3f{ 0.2f, 0.2f, 0.2f }); // highlighted fill
	}
	else 
	{
		draw_fill(params, _min, _max, ga_vec3f{ 0.05f, 0.05f, 0.05f }); // regular fill
	}

	extern ga_font* g_font; // create font object
	g_font->print(params, text, _max.x + 10, _max.y - 8, k_text_color); // text
}

ga_checkbox::~ga_checkbox()
{
}

bool ga_checkbox::get_hover(const ga_frame_params* params) const
{
	// TODO: Homework 4
	return params->_mouse_press_mask == 0 &&
		params->_mouse_click_mask == 0 &&
		mouse_in_range(params, _min, _max);
}

bool ga_checkbox::get_pressed(const ga_frame_params* params) const
{
	// TODO: Homework 4
	return params->_mouse_press_mask != 0 &&
		mouse_in_range(params, _min, _max);
}

bool ga_checkbox::get_clicked(const ga_frame_params* params) const
{
	// TODO: Homework 4
	return params->_mouse_click_mask != 0 &&
		mouse_in_range(params, _min, _max);
}
