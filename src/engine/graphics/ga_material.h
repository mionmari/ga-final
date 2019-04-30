#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_program.h"
#include "ga_texture.h"
#include "framework/ga_frame_params.h"

#include "math/ga_mat4f.h"
#include "math/ga_vec3f.h"

#include <string>

/*
** Base class for all graphical materials.
** Includes the shaders and other state necessary to draw geometry.
*/
class ga_material
{
public:
	virtual bool init() = 0;

	virtual void bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params) = 0;

	virtual void set_color(const ga_vec3f& color) {}
};

/*
** Simple untextured material with a constant color.
*/
class ga_constant_color_material : public ga_material
{
public:
	ga_constant_color_material();
	~ga_constant_color_material();

	virtual bool init() override;

	virtual void bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params) override;

	virtual void set_color(const ga_vec3f& color) override { _color = color; }

private:
	ga_shader * _vs;
	ga_shader* _fs;
	ga_program* _program;
	ga_vec3f _color;
};

/*
** Simple unlit, single textured material.
*/
class ga_unlit_texture_material : public ga_material
{
public:
	ga_unlit_texture_material(const char* texture_file);
	~ga_unlit_texture_material();

	virtual bool init() override;

	virtual void bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params) override;

private:
	std::string _texture_file;

	ga_shader* _vs;
	ga_shader* _fs;
	ga_program* _program;
	ga_texture* _texture;
	std::chrono::high_resolution_clock::time_point _start_time = std::chrono::high_resolution_clock::now();
};

/*
** lit, untextured material.
*/
class ga_lit_material : public ga_material
{
public:
	ga_lit_material();
	~ga_lit_material();

	virtual bool init() override;
	virtual void bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params) override;
	void set_diffuse(ga_vec3f& color) { _color = color; }
	void set_diffuse(float r, float g, float b) { _color.x = r; _color.y = g; _color.z = b; }

private:
	ga_shader* _vs;
	ga_shader* _fs;
	ga_program* _program;
	ga_vec3f _color;

};

/*
** lit, single textured material.
*/
class ga_lit_texture_material : public ga_material
{
public:
	ga_lit_texture_material();
	ga_lit_texture_material(const char* texture_file);

	~ga_lit_texture_material();

	virtual bool init() override;

	virtual void bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params) override;

private:
	std::string _texture_file;

	ga_shader* _vs;
	ga_shader* _fs;
	ga_program* _program;
	ga_texture* _texture;
	std::chrono::high_resolution_clock::time_point _start_time = std::chrono::high_resolution_clock::now();
};


/*
** PBR material.
*/
class ga_pbr_material : public ga_material
{
public:
	ga_pbr_material();
	ga_pbr_material
	(
		const char* albedo,
		const char* normal,
		const char* metallic,
		const char* roughness,
		const char* ao
	);

	~ga_pbr_material();

	virtual bool init() override;

	virtual void bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params) override;

private:
	std::string _albedo_file;
	std::string _normal_file;
	std::string _metallic_file;
	std::string _roughness_file;
	std::string _ao_file;

	ga_shader* _vs;
	ga_shader* _fs;
	ga_program* _program;

	ga_texture* _albedo;
	ga_texture* _normal;
	ga_texture* _metallic;
	ga_texture* _roughness;
	ga_texture* _ao;

	std::chrono::high_resolution_clock::time_point _start_time = std::chrono::high_resolution_clock::now();
};
