#include "graphics.h"

t_vec3f	vec3_sub(t_vec3f v1, t_vec3f v2)
{
	t_vec3f	vec;

	vec.x = v1.x - v2.x;
	vec.y = v1.y - v2.y;
	vec.z = v1.z - v2.z;
	return (vec);
}
