#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "math/ga_vec3f.h"
#include "math/ga_vec4f.h"
#include "math/ga_math.h"

/*
** Floating point quaternion object.
*/
struct ga_quatf
{
	union
	{
		struct { float x, y, z, w; };
		struct { ga_vec3f v3; float s; };
		ga_vec4f v4;
		float axes[4];
	};

	/*
	** Build a quaternion for an axis and angle.
	** @param result On return, the quaternion representing the axis and angle.
	** @param axis Normalized axis.
	** @param angle Angle about axis in radians.
	*/
	inline void make_axis_angle(const ga_vec3f& __restrict axis, float angle)
	{
		s = ga_cosf(angle * 0.5f);
		v3 = axis.scale_result(ga_sinf(angle * 0.5f));
	}

	/*
	** Multiply two quaternions.
	** @param result On return, the quaternion representing a * b.
	** @param a The first quaternion.
	** @param b The second quaternion.
	*/
	inline ga_quatf operator*(const ga_quatf& __restrict b) const
	{
		ga_quatf result;
		result.v3 = ga_vec3f_cross(v3, b.v3);

		ga_vec3f tmp1;
		tmp1 = b.v3.scale_result(s);
		result.v3 += tmp1;
		ga_vec3f tmp2;
		tmp2 = v3.scale_result(b.s);
		result.v3 += tmp2;

		result.s = (s * b.s) - v3.dot(b.v3);
	}

	/*
	** Conjugate a quaternion in place.
	** @param q The quaternion.
	*/
	inline void conjugate()
	{
		v3.negate();
	}

	/*
	** Invert a quaternion in place.
	** @param q The quaternion.
	*/
	inline void inverse()
	{
		conjugate();
		v4.scale(1.0f / v4.mag2());
	}
};
