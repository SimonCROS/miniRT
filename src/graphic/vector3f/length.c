#include <math.h>
#include "graphics.h"

float	vec3_length_squared(t_vec3f v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

float	vec3_length(t_vec3f v)
{
	return (sqrtf(vec3_length_squared(v)));
}
