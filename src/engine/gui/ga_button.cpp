/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_button.h"
#include "ga_font.h"

#include "framework/ga_frame_params.h"

ga_button::ga_button(const char* text, float x, float y, ga_frame_params* params)
{
	// TODO: Homework 4

	// Compute min/max
	_min = ga_vec2f{ x, y };
	_max = ga_vec2f{ x + 170, y + 30 };

	// Draw the button
	draw_outline(params, _min, _max, k_button_color, k_button_offset); // outline
	extern ga_font* g_font; // create font object

	// fill color
	ga_vec3f color = k_button_color;
	if (get_hover(params)) 
	{
		color = k_button_hover_color;
	}
	else if (get_pressed(params))
	{
		color = k_button_press_color;
	}
	draw_fill(params, _min, _max, color);

	g_font->print(params, text, x + 10, y + 22, k_text_color); // text
}

ga_button::~ga_button()
{
}

bool ga_button::get_hover(const ga_frame_params* params) const
{
	// TODO: Homework 4
	return params->_mouse_press_mask == 0 &&
		params->_mouse_click_mask == 0 &&
		mouse_in_range(params, _min, _max);
}

bool ga_button::get_pressed(const ga_frame_params* params) const
{
	// TODO: Homework 4
	return params->_mouse_press_mask != 0 &&
		mouse_in_range(params, _min, _max);
}

bool ga_button::get_clicked(const ga_frame_params* params) const
{
	// TODO: Homework 4
	return params->_mouse_click_mask != 0 && 
		mouse_in_range(params, _min, _max);
}
