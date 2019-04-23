#include "graphics/ga_node.h"
#include "graphics/ga_mesh.h"
#include "framework/ga_frame_params.h"

ga_node::ga_node(ga_node* parent) 
{ 
	_parent = parent;
	_transform.make_identity(); 
}
ga_node::ga_node() 
{ 
	_parent = nullptr; 
	_transform.make_identity(); 
}

void ga_node::update(ga_frame_params* params, ga_mat4f parent_transform)
{
	_world_transform = _transform * parent_transform;
	for (ga_node* n : _children)
	{
		n->update(params,_world_transform);
	}
}


void ga_node::draw_recursive(ga_frame_params* params)
{
	for (ga_mesh* m : _meshes)
	{
		ga_static_drawcall draw;

		draw._name = _name;
		m->assemble_drawcall(draw);
		if (m->_skeleton._joints.size() > 0)
		{
			draw._transform.make_identity();
		}
		else
		{
			draw._transform = _world_transform;
		}
		while (params->_static_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
		params->_static_drawcalls.push_back(draw);
		params->_static_drawcall_lock.clear(std::memory_order_release);
	}

	for (ga_node* n : _children)
	{
		n->draw_recursive(params);
	}

}

// recursive find by name
ga_node* ga_node::find(std::string target_name)
{
	ga_node* result = nullptr;
	if (_name == target_name)
	{
		result = this;
	}
	else
	{
		for (ga_node* child : _children)
		{
			result = child->find(target_name);
			if (result != nullptr)
			{
				break;
			}
		}
	}
	return result;
}