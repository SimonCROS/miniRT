#include "graphics.h"

float	vec3_dot(t_vec3f v1, t_vec3f v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}
