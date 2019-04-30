/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.

working devel version
*/
#include "framework/ga_camera.h"
#include "framework/ga_compiler_defines.h"
#include "framework/ga_input.h"
#include "framework/ga_sim.h"
#include "framework/ga_output.h"
#include "jobs/ga_job.h"
#include "entity/ga_entity.h"
#include "graphics/ga_cube_component.h"
#include "graphics/ga_model_component.h"
#include "graphics/ga_program.h"

#include "gui/ga_font.h"
#include "gui/ga_button.h"
#include "gui/ga_checkbox.h"
#include "gui/ga_label.h"

/* assimp include files. These three are usually needed. */

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <iostream>

#if defined(GA_MINGW)
#include <unistd.h>
#endif

ga_font* g_font = nullptr;

static void gui_test(ga_frame_params* params);

char g_root_path[256];
static void set_root_path(const char* exepath);

ga_entity sphere_rust;
ga_entity sphere_grass;
ga_entity sphere_wood;

ga_entity couch_rust;
ga_entity couch_grass;
ga_entity couch_wood;

int model_sel = 0;
int mat_sel = 0;

int main(int argc, const char** argv)
{
	set_root_path(argv[0]);

	ga_job::startup(0xffff, 256, 256);

	// Create objects for three phases of the frame: input, sim and output.
	ga_input* input = new ga_input();
	ga_sim* sim = new ga_sim();
	ga_output* output = new ga_output(input->get_window());

	// Create camera.
	ga_entity camera_ent;
	ga_camera* camera = new ga_camera(&camera_ent);

	camera_ent.translate({ 0.0f, 2.0f, 7.0f });
	camera_ent.rotate({ -15.0f, 180.0f, 0 });
	sim->add_entity(&camera_ent);

	// save material maps
	struct ga_maps rust_maps;
	rust_maps.albedo_map = "data/textures/rustediron1-alt2-Unreal-Engine/rustediron2_basecolor.png";
	rust_maps.normal_map = "data/textures/rustediron1-alt2-Unreal-Engine/rustediron2_normal.png";
	rust_maps.metallic_map = "data/textures/rustediron1-alt2-Unreal-Engine/rustediron2_metallic.png";
	rust_maps.roughness_map = "data/textures/rustediron1-alt2-Unreal-Engine/rustediron2_roughness.png";
	rust_maps.ao_map = "data/textures/rustediron1-alt2-Unreal-Engine/rustediron2_ao.png";

	struct ga_maps grass_maps;
	grass_maps.albedo_map = "data/textures/grass1/grass1-albedo3.png";
	grass_maps.normal_map = "data/textures/grass1/grass1-normal1-dx.png";
	grass_maps.metallic_map = "data/textures/grass1/grass1-metal.png";
	grass_maps.roughness_map = "data/textures/grass1/grass1-rough.png";
	grass_maps.ao_map = "data/textures/grass1/grass1-ao.png";

	struct ga_maps wood_maps;
	wood_maps.albedo_map = "data/textures/bamboo-wood-semigloss-Unreal-Engine/bamboo-wood-semigloss-albedo.png";
	wood_maps.normal_map = "data/textures/bamboo-wood-semigloss-Unreal-Engine/bamboo-wood-semigloss-normal.png";
	wood_maps.metallic_map = "data/textures/bamboo-wood-semigloss-Unreal-Engine/bamboo-wood-semigloss-metal.png";
	wood_maps.roughness_map = "data/textures/bamboo-wood-semigloss-Unreal-Engine/bamboo-wood-semigloss-roughness.png";
	wood_maps.ao_map = "data/textures/bamboo-wood-semigloss-Unreal-Engine/bamboo-wood-semigloss-ao.png";

	
	// sphere models
	ga_model_component sphere_rust_mesh(&sphere_rust, "data/models/sphere_smooth.obj", &rust_maps);
	sphere_rust.set_active(true);
	sim->add_entity(&sphere_rust);

	ga_model_component sphere_grass_mesh(&sphere_grass, "data/models/sphere_smooth.obj", &grass_maps);
	sphere_grass.set_active(false);
	sim->add_entity(&sphere_grass);

	ga_model_component sphere_wood_mesh(&sphere_wood, "data/models/sphere_smooth.obj", &wood_maps);
	sphere_wood.set_active(false);
	sim->add_entity(&sphere_wood);

	// torus models
	ga_model_component couch_rust_mesh(&couch_rust, "data/models/couch.obj", &rust_maps);
	couch_rust.set_active(false);
	sim->add_entity(&couch_rust);

	ga_model_component couch_grass_mesh(&couch_grass, "data/models/couch.obj", &grass_maps);
	couch_grass.set_active(false);
	sim->add_entity(&couch_grass);

	ga_model_component couch_wood_mesh(&couch_wood, "data/models/couch.obj", &wood_maps);
	couch_wood.set_active(false);
	sim->add_entity(&couch_wood);


	// Create the default font:
	g_font = new ga_font("data/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 24.0f, 512, 512);

	// Main loop:
	while (true)
	{
		ga_frame_params params;

		params._render_settings = &output->_render_settings;

		if (!input->update(&params))
		{
			break;
		}

		// Run gameplay.
		sim->update(&params);

		// Perform the late update.
		sim->late_update(&params);

		// Run gui test.
		gui_test(&params);

		// Draw to screen.
		output->update(&params);
	}

	delete output;
	delete sim;
	delete input;
	delete camera;

	ga_job::shutdown();
	return 0;
}


static void gui_test(ga_frame_params* params)
{
	ga_label("Model Viewer", 20, 100, params);
	ga_label("Material", 20, 175, params);
	ga_label("Model", 1050, 175, params);

	if (ga_checkbox(params->_render_settings->_lighting_enabled, "Enable Lighting", 20.0f, 200.0f, params).get_clicked(params))
	{
		// TODO: Homework 4
		// toggle lighting 
		params->_render_settings->_lighting_enabled = !params->_render_settings->_lighting_enabled;
	}	
	
	// Material buttons
	if (ga_button("Rusted Metal", 20.0f, 250.0f, params).get_clicked(params))
	{
		mat_sel = 0;
	}
	if (ga_button("Grass", 20.0f, 300.0f, params).get_clicked(params))
	{
		mat_sel = 1;
	}
	if (ga_button("Glossy Wood", 20.0f, 350.0f, params).get_clicked(params))
	{
		mat_sel = 2;
	}

	// Model butttons
	if (ga_button("Sphere", 1050.0f, 200.0f, params).get_clicked(params))
	{
		model_sel = 0;
	}
	if (ga_button("Couch", 1050.0f, 250.0f, params).get_clicked(params))
	{
		model_sel = 1;
	}

	sphere_rust.set_active(false);
	sphere_grass.set_active(false);
	sphere_wood.set_active(false);

	couch_rust.set_active(false);
	couch_grass.set_active(false);
	couch_wood.set_active(false);

	if (model_sel == 0) 
	{
		if (mat_sel == 0) 
		{
			sphere_rust.set_active(true);
		}
		else if (mat_sel == 1) 
		{
			sphere_grass.set_active(true);
		}
		else if (mat_sel == 2)
		{
			sphere_wood.set_active(true);
		}
	}
	else if (model_sel == 1) 
	{
		if (mat_sel == 0)
		{
			couch_rust.set_active(true);
		}
		else if (mat_sel == 1)
		{
			couch_grass.set_active(true);
		}
		else if (mat_sel == 2)
		{
			couch_wood.set_active(true);
		}
	}
}


static void set_root_path(const char* exepath)
{
#if defined(GA_MSVC)
	strcpy_s(g_root_path, sizeof(g_root_path), exepath);

	// Strip the executable file name off the end of the path:
	char* slash = strrchr(g_root_path, '\\');
	if (!slash)
	{
		slash = strrchr(g_root_path, '/');
	}
	if (slash)
	{
		slash[1] = '\0';
	}
#elif defined(GA_MINGW)
	char* cwd;
	char buf[PATH_MAX + 1];
	cwd = getcwd(buf, PATH_MAX + 1);
	strcpy_s(g_root_path, sizeof(g_root_path), cwd);

	g_root_path[strlen(cwd)] = '/';
	g_root_path[strlen(cwd) + 1] = '\0';
#endif
}
