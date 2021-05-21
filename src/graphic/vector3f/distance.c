#include <math.h>
#include "graphics.h"

float	vec3_distance_squared(t_vec3f v1, t_vec3f v2)
{
	float	dx;
	float	dy;
	float	dz;

	dx = v1.x - v2.x;
	dy = v1.y - v2.y;
	dz = v1.z - v2.z;
	return (fabsf(dx * dx + dy * dy + dz * dz));
}

float	vec3_distance(t_vec3f v1, t_vec3f v2)
{
	return (sqrtf(vec3_distance_squared(v1, v2)));
}
