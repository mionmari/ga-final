#pragma once
#include <vector>
/*
*	joint 
*/
struct ga_joint
{
	std::string _name;
	class ga_node* _node_ptr= nullptr;
	struct ga_mat4f _bind_matrix;
	struct ga_mat4f _inv_bind_matrix;
	struct ga_mat4f _world_matrix;
	struct ga_mat4f _skin_matrix;
};

/*
** A skeleton made up of multiple joints.
** The joints are stored in a vector in the order they are parsed.
*/
struct ga_skeleton
{
	static const uint32_t k_max_skeleton_joints = 75;

	std::vector<ga_joint*> _joints;
};