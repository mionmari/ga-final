#include "graphics/ga_animation.h"
#include "graphics/ga_node.h"

#include <iostream>

float lerp(float v1, float v2, float t)
{
	// TODO: Homework 6.
	// implement LERP for single float and 2,3, and 4 dimensional vectors
	return (v1 * (1.0f - t)) + (v2 * t);
}

ga_vec2f lerp(ga_vec2f v1, ga_vec2f v2, float t)
{
	// TODO: Homework 6.
	return v1.scale_result(1.0f - t) + v2.scale_result(t);
}
ga_vec3f lerp(ga_vec3f v1, ga_vec3f v2, float t)
{
	// TODO: Homework 6.
	return v1.scale_result(1.0f - t) + v2.scale_result(t);
}
ga_vec4f lerp(ga_vec4f v1, ga_vec4f v2, float t)
{
	// TODO: Homework 6.
	return v1.scale_result(1.0f - t) + v2.scale_result(t);
}

ga_quatf slerp(ga_quatf q1, ga_quatf q2, float t)
{
	// TODO: Homework 6.
	// implement SLERP for quaternions

	// If quaternions are equivalent, do not slerp
	if (q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w) return q1;
	
	// Compute dot product
	float dot = q1.dot(q2);
	if (dot > 1.0f) 
	{
		dot = 1.0f;
	}
	else if (dot < -1.0f) 
	{
		dot = -1.0f;
	}

	// Commpute theta
	float theta = acosf(dot);

	float sin_of_theta = sinf(theta);
	float q1_weight = sinf((1 - t) * theta) / sin_of_theta;
	float q2_weight = sinf(t * theta) / sin_of_theta;
	
	// Apply weights
	ga_quatf q3 = q1.scale_result(q1_weight) + q2.scale_result(q2_weight);

	// Do not normalize zero quaternion
	if (q3.x != 0.0f && q3.y != 0.0f && q3.z != 0.0f && q3.w != 0.0f) 
	{
		q3.normalize();
	}
	return q3;
}

ga_quatf nlerp(ga_quatf q1, ga_quatf q2, float t)
{
	// TODO: Homework 6.
	// implement NLERP for quaternions
	ga_quatf q3 = q1.scale_result(1 - t) + q2.scale_result(t);

	// Do not normalize zero quaternion
	if (q3.x != 0.0f && q3.y != 0.0f && q3.z != 0.0f && q3.w != 0.0f)
	{
		q3.normalize();
	}
	return q3;
}

ga_animation_player::ga_animation_player()
{
	_current_clip = nullptr;
	_playing = false;
	_elapsed_time = 0;

}
ga_animation_player::~ga_animation_player()
{

}

void ga_animation_player::add_clip(ga_animclip* clip)
{
	_clips.push_back(clip);
}

void ga_animation_player::play(ga_animclip* clip)
{
	_current_clip = clip;
	_playing = true;
	_elapsed_time = 0;
}

void ga_animation_player::play(int clip_idx)
{
	if (clip_idx < _clips.size())
	{
		_current_clip = _clips[clip_idx];
		_playing = true;
		_elapsed_time = 0;
	}
}
void ga_animation_player::play()
{
	_playing = true;
}
void ga_animation_player::pause()
{
	_playing = false;
}
bool ga_animation_player::is_playing()
{
	return _playing;
}
ga_animclip* ga_animation_player::get_current_clip()
{
	return _current_clip;
}
float ga_animation_player::get_time()
{
	return _elapsed_time;
}
float ga_animation_player::get_current_frame()
{
	// TODO: Homework 6.
	// convert seconds to frames:
	// return the current frame number based on the player's elapsed time

	// No clip playing, return frame 0
	if (_current_clip == NULL)
	{
		return 0.0f;
	}
	
	return trunc(_current_clip->_fps * _elapsed_time);
}


void ga_animation_player::update(float dt)
{
	// TODO: Homework 6.
	// If an animation is playing, update local time and determine the current frame.
	// Using the pose of the current frame, you must calculate each node's new world
	// and skinning matrices.

	// Check if animation is playing
	if (!_playing) return;

	// Update local time
	_elapsed_time += dt;
	// If elapsed time greater than clip length, loop back to 0.
	if (_elapsed_time > (_current_clip->_duration / _current_clip->_fps))
	{
		_elapsed_time = 0;
	}

	// Determine the current frame
	float frame = get_current_frame();

	// update world transform of each node
	std::vector<ga_animchannel*> channels = _current_clip->_channels;

	for (int i = 0; i < channels.size(); ++i)
	{
		ga_node *node = channels[i]->_node;
		std::vector<ga_animkey> keys = channels[i]->_keys;

		if (keys.size() == 0) continue; // no keys to animate

		// Lerp between 2 keys
		ga_animkey prevKey = keys[0];
		ga_animkey nextKey = keys[1];

		// Find which two keys the current frame is between
		for (int j = 2; j < keys.size(); ++j)
		{
			if (frame > keys[j - 1]._time)
			{
				prevKey = keys[j - 1];
				nextKey = keys[j];
			}
		}

		float framesDone = frame - prevKey._time;
		float totalFrames = nextKey._time - prevKey._time;
		float t = framesDone / totalFrames; // t value for interpolation

		// Current frame is later than last key frame (use last key frame values)
		if (frame > keys[keys.size() - 1]._time) 
		{
			t = 1;
		}

		// Create new transformation matrix
		ga_mat4f newTransform;
		ga_vec3f newTranslation = lerp(prevKey._translation, nextKey._translation, t);
		ga_quatf newRotation = slerp(prevKey._rotation, nextKey._rotation, t);

		newTransform.make_rotation(newRotation);
		newTransform.set_translation(newTranslation);
		node->_transform = newTransform;
	}
}
