/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_material.h"
//#include "ga_frame_params.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

void load_shader(const char* filename, std::string& contents)
{
	extern char g_root_path[256];
	std::string fullpath = g_root_path;
	fullpath += filename;

	std::ifstream file(fullpath);

	assert(file.is_open());

	contents.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

ga_constant_color_material::ga_constant_color_material()
{
}

ga_constant_color_material::~ga_constant_color_material()
{
}

bool ga_constant_color_material::init()
{
	std::string source_vs;
	load_shader("data/shaders/ga_constant_color_vert.glsl", source_vs);

	std::string source_fs;
	load_shader("data/shaders/ga_constant_color_frag.glsl", source_fs);

	_vs = new ga_shader(source_vs.c_str(), GL_VERTEX_SHADER);
	if (!_vs->compile())
	{
		std::cerr << "Failed to compile vertex shader:" << std::endl << _vs->get_compile_log() << std::endl;
	}

	_fs = new ga_shader(source_fs.c_str(), GL_FRAGMENT_SHADER);
	if (!_fs->compile())
	{
		std::cerr << "Failed to compile fragment shader:\n\t" << std::endl << _fs->get_compile_log() << std::endl;
	}

	_program = new ga_program();
	_program->attach(*_vs);
	_program->attach(*_fs);
	if (!_program->link())
	{
		std::cerr << "Failed to link shader program:\n\t" << std::endl << _program->get_link_log() << std::endl;
	}

	_color = { 0.8f, 0.8f, 0.8f };
	return true;
}

void ga_constant_color_material::bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params)
{
	ga_uniform mvp_uniform = _program->get_uniform("u_mvp");
	ga_uniform color_uniform = _program->get_uniform("u_color");

	_program->use();

	mvp_uniform.set(transform * view_proj);
	color_uniform.set(_color);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

ga_unlit_texture_material::ga_unlit_texture_material(const char* texture_file) :
	_texture_file(texture_file)
{
}

ga_unlit_texture_material::~ga_unlit_texture_material()
{
}

bool ga_unlit_texture_material::init()
{
	std::string source_vs;
	load_shader("data/shaders/ga_unlit_texture_vert.glsl", source_vs);

	std::string source_fs;
	load_shader("data/shaders/ga_unlit_texture_frag.glsl", source_fs);

	_vs = new ga_shader(source_vs.c_str(), GL_VERTEX_SHADER);
	if (!_vs->compile())
	{
		std::cerr << "Failed to compile vertex shader:" << std::endl << _vs->get_compile_log() << std::endl;
	}

	_fs = new ga_shader(source_fs.c_str(), GL_FRAGMENT_SHADER);
	if (!_fs->compile())
	{
		std::cerr << "Failed to compile fragment shader:\n\t" << std::endl << _fs->get_compile_log() << std::endl;
	}

	_program = new ga_program();
	_program->attach(*_vs);
	_program->attach(*_fs);
	if (!_program->link())
	{
		std::cerr << "Failed to link shader program:\n\t" << std::endl << _program->get_link_log() << std::endl;
	}

	_texture = new ga_texture();
	if (!_texture->load_from_file(_texture_file.c_str()))
	{
		std::cerr << "Failed to load texture: " << _texture_file << std::endl;
	}

	return true;
}

void ga_unlit_texture_material::bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params)
{
	ga_uniform mvp_uniform = _program->get_uniform("u_mvp");
	ga_uniform texture_uniform = _program->get_uniform("u_texture");
	ga_uniform time_uniform = _program->get_uniform("u_time");

	_program->use();

	mvp_uniform.set(transform * view_proj);
	texture_uniform.set(*_texture, 0);
	time_uniform.set(std::chrono::duration_cast<std::chrono::duration<float>>(params->_current_time - _start_time).count());
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}



ga_lit_texture_material::ga_lit_texture_material(const char* texture_file) :
	_texture_file(texture_file)
{
}
ga_lit_texture_material::ga_lit_texture_material()
{
	_texture_file = "";
}

ga_lit_texture_material::~ga_lit_texture_material()
{
}

bool ga_lit_texture_material::init()
{
	std::string source_vs;
	load_shader("data/shaders/ga_lit_texture_vert.glsl", source_vs);

	std::string source_fs;
	load_shader("data/shaders/ga_lit_textur_frag.glsl", source_fs);

	_vs = new ga_shader(source_vs.c_str(), GL_VERTEX_SHADER);
	if (!_vs->compile())
	{
		std::cerr << "Failed to compile vertex shader:" << std::endl << _vs->get_compile_log() << std::endl;
	}

	_fs = new ga_shader(source_fs.c_str(), GL_FRAGMENT_SHADER);
	if (!_fs->compile())
	{
		std::cerr << "Failed to compile fragment shader:\n\t" << std::endl << _fs->get_compile_log() << std::endl;
	}

	_program = new ga_program();
	_program->attach(*_vs);
	_program->attach(*_fs);
	if (!_program->link())
	{
		std::cerr << "Failed to link shader program:\n\t" << std::endl << _program->get_link_log() << std::endl;
	}

	_texture = new ga_texture();
	if (!_texture->load_from_file(_texture_file.c_str()))
	{
		std::cerr << "Failed to load texture: " << _texture_file << std::endl;
	}

	return true;
}

void ga_lit_texture_material::bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params)
{
	ga_uniform mvp_uniform = _program->get_uniform("u_mvp");
	ga_uniform texture_uniform = _program->get_uniform("u_texture");
	ga_uniform time_uniform = _program->get_uniform("u_time");

	_program->use();

	mvp_uniform.set(transform * view_proj);
	texture_uniform.set(*_texture, 0);
	time_uniform.set(std::chrono::duration_cast<std::chrono::duration<float>>(params->_current_time - _start_time).count());
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

ga_lit_material::ga_lit_material()
{
	_color = { 1.0f, 1.0f, 1.0f };
}
ga_lit_material::~ga_lit_material()
{
}

bool ga_lit_material::init()
{
	std::string source_vs;
	load_shader("data/shaders/ga_lit_vert.glsl", source_vs);

	std::string source_fs;
	load_shader("data/shaders/ga_lit_frag.glsl", source_fs);

	_vs = new ga_shader(source_vs.c_str(), GL_VERTEX_SHADER);
	if (!_vs->compile())
	{
		std::cerr << "Failed to compile vertex shader:" << std::endl << _vs->get_compile_log() << std::endl;
	}

	_fs = new ga_shader(source_fs.c_str(), GL_FRAGMENT_SHADER);
	if (!_fs->compile())
	{
		std::cerr << "Failed to compile fragment shader:\n\t" << std::endl << _fs->get_compile_log() << std::endl;
	}

	_program = new ga_program();
	_program->attach(*_vs);
	_program->attach(*_fs);
	if (!_program->link())
	{
		std::cerr << "Failed to link shader program:\n\t" << std::endl << _program->get_link_log() << std::endl;
	}

	return true;
}

void ga_lit_material::bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params)
{
	ga_uniform mvp_uniform = _program->get_uniform("u_mvp");
	ga_uniform diffuse_uniform = _program->get_uniform("u_diffuse");
	ga_uniform lighting_enabled_uniform = _program->get_uniform("u_lighting_enabled");

	_program->use();

	mvp_uniform.set(transform * view_proj);
	diffuse_uniform.set(_color);
	lighting_enabled_uniform.set(params->_render_settings->_lighting_enabled);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

ga_pbr_material::ga_pbr_material
(
	const char* albedo,
	const char* normal,
	const char* metallic,
	const char* roughness,
	const char* ao
) :
	_albedo_file(albedo),
	_normal_file(normal),
	_metallic_file(metallic),
	_roughness_file(roughness),
	_ao_file(ao)
{
}

ga_pbr_material::ga_pbr_material()
{
	_albedo_file = "";
	_normal_file = "";
	_metallic_file = "";
	_roughness_file = "";
	_ao_file = "";
}

ga_pbr_material::~ga_pbr_material()
{
}

bool ga_pbr_material::init()
{
	std::string source_vs;
	load_shader("data/shaders/ga_pbr_vert.glsl", source_vs);

	std::string source_fs;
	load_shader("data/shaders/ga_pbr_frag.glsl", source_fs);

	_vs = new ga_shader(source_vs.c_str(), GL_VERTEX_SHADER);
	if (!_vs->compile())
	{
		std::cerr << "Failed to compile vertex shader:" << std::endl << _vs->get_compile_log() << std::endl;
	}

	_fs = new ga_shader(source_fs.c_str(), GL_FRAGMENT_SHADER);
	if (!_fs->compile())
	{
		std::cerr << "Failed to compile fragment shader:\n\t" << std::endl << _fs->get_compile_log() << std::endl;
	}

	_program = new ga_program();
	_program->attach(*_vs);
	_program->attach(*_fs);
	if (!_program->link())
	{
		std::cerr << "Failed to link shader program:\n\t" << std::endl << _program->get_link_log() << std::endl;
	}

	_albedo = new ga_texture();
	if (!_albedo->load_from_file(_albedo_file.c_str()))
	{
		std::cerr << "Failed to load albedo map: " << _albedo_file << std::endl;
	}

	_normal = new ga_texture();
	if (!_normal->load_from_file(_normal_file.c_str()))
	{
		std::cerr << "Failed to load normal map: " << _normal_file << std::endl;
	}

	_metallic = new ga_texture();
	if (!_metallic->load_from_file(_metallic_file.c_str()))
	{
		std::cerr << "Failed to load metallic map: " << _metallic_file << std::endl;
	}

	_roughness = new ga_texture();
	if (!_roughness->load_from_file(_roughness_file.c_str()))
	{
		std::cerr << "Failed to load roughness map: " << _roughness_file << std::endl;
	}

	_ao = new ga_texture();
	if (!_ao->load_from_file(_ao_file.c_str()))
	{
		std::cerr << "Failed to load ao map: " << _ao_file << std::endl;
	}

	return true;
}

void ga_pbr_material::bind(const ga_mat4f& view_proj, const ga_mat4f& transform, ga_frame_params* params)
{
	ga_uniform mvp_uniform = _program->get_uniform("u_mvp");
	ga_uniform world_mat_uniform = _program->get_uniform("u_world");
	ga_uniform cam_pos_uniform = _program->get_uniform("u_cam_pos");
	ga_uniform lighting_enabled_uniform = _program->get_uniform("u_lighting_enabled");

	ga_uniform albedo_uniform = _program->get_uniform("u_albedo");
	//ga_uniform normal_uniform = _program->get_uniform("u_normal");
	ga_uniform metallic_uniform = _program->get_uniform("u_metallic");
	ga_uniform roughness_uniform = _program->get_uniform("u_roughness");
	ga_uniform ao_uniform = _program->get_uniform("u_ao");

	_program->use();

	mvp_uniform.set(transform * view_proj);
	world_mat_uniform.set(transform);
	lighting_enabled_uniform.set(params->_render_settings->_lighting_enabled);
	cam_pos_uniform.set(params->_cam_pos);

	albedo_uniform.set(*_albedo, 0);
	//normal_uniform.set(*_normal, 1);
	metallic_uniform.set(*_metallic, 1);
	roughness_uniform.set(*_roughness, 2);
	ao_uniform.set(*_ao, 3);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}