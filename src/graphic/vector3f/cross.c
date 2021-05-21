#include "graphics.h"

t_vec3f	vec3_cross(t_vec3f v1, t_vec3f v2)
{
	t_vec3f	vec;

	vec.x = v1.y * v2.z - v1.z * v2.y;
	vec.y = v1.z * v2.x - v1.x * v2.z;
	vec.z = v1.x * v2.y - v1.y * v2.x;
	return (vec);
}
