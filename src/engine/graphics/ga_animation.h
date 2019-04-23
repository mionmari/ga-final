#pragma once
/*
**	animation key
*/

#include <string>
#include <vector>
#include "math/ga_quatf.h"
#include "math/ga_vec2f.h"
#include "math/ga_vec3f.h"
#include "math/ga_vec4f.h"
#include "math/ga_mat4f.h"

float lerp(float v1, float v2, float t);
ga_vec2f lerp(ga_vec2f v1, ga_vec2f v2, float t);
ga_vec3f lerp(ga_vec3f v1, ga_vec3f v2, float t);
ga_vec4f lerp(ga_vec4f v1, ga_vec4f v2, float t);
ga_quatf slerp(ga_quatf q1, ga_quatf q2, float t);
ga_quatf nlerp(ga_quatf q1, ga_quatf q2, float t);

/*
** animation key
*/
struct ga_animkey
{
	float _time;
	ga_quatf _rotation;
	ga_vec3f _translation;
	ga_vec3f _scale;
};

/* 
** animation channel
** contains a pointer to the node it animates
** and a vector of keys
*/

struct ga_animchannel
{
	class ga_node* _node;
	std::vector<ga_animkey> _keys;
};

/*
** animation clip
*/
struct ga_animclip
{
	std::string _name;
	std::vector<ga_animchannel*> _channels;
	float _duration;
	float _fps;
	float _current_time = 0.0f;
};

/*
** animation player
*/

class ga_animation_player
{
public:
	ga_animation_player();
	~ga_animation_player();

	void add_clip(ga_animclip* clip);
	void play(ga_animclip* clip);
	void play(int i);
	void play();
	void stop();
	void pause();
	void update(float dt);
	bool is_playing();
	ga_animclip* get_current_clip();
	float get_time();
	float get_current_frame();

private:
	std::vector<ga_animclip*> _clips;
	ga_animclip* _current_clip;
	float _elapsed_time;
	bool _playing;
};