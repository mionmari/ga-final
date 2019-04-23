#pragma once
/*
** scenegraph node 
*/

#include <vector>
#include <string>
#include "graphics/ga_mesh.h"
class ga_node
{
public:
	ga_node(ga_node* parent);
	ga_node();

	void update(struct ga_frame_params *params, struct ga_mat4f parent_transform);
	void draw_recursive(struct ga_frame_params* params);

	ga_node* find(std::string name);
	ga_mat4f _transform;
	ga_mat4f _world_transform;
	std::string _name;
	ga_node* _parent;
	std::vector<ga_node*> _children;
	std::vector<class ga_mesh*> _meshes;
};

