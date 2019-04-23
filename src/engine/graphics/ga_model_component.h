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
#include "math/ga_quatf.h"
#include "math/ga_vec3f.h"
#include "graphics/ga_mesh.h"
#include "graphics/ga_node.h"
#include "graphics/ga_animation.h"

#include <assimp/scene.h>           // Output data structure

#include <cstdint>
#include <vector>
#include <string>


class ga_model_component : public ga_component
{
public:
	ga_model_component(class ga_entity* ent, const char* model_file);
	virtual ~ga_model_component();

	virtual void update(struct ga_frame_params* params) override;
	void process_node_recursive(aiNode* ai_node, const aiScene* scene, struct ga_node* node);

private:

	std::vector<ga_mesh*> _meshes;
	const aiScene *_scene;
	ga_node _root;
	ga_animation_player _animation;
};


