#include "graphics.h"

t_vec3f	vec3_muld(t_vec3f v, float a)
{
	t_vec3f	vec;

	vec.x = v.x * a;
	vec.y = v.y * a;
	vec.z = v.z * a;
	return (vec);
}

t_vec3f	vec3_mul(t_vec3f v1, t_vec3f v2)
{
	t_vec3f	vec;

	vec.x = v1.x * v2.x;
	vec.y = v1.y * v2.y;
	vec.z = v1.z * v2.z;
	return (vec);
}
