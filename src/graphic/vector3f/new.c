#include "graphics.h"

t_vec3f	vec3_null(void)
{
	return ((t_vec3f){0, 0, 0});
}

int	vec3_is_null(t_vec3f v)
{
	return (vec3_length_squared(v) == 0);
}

t_vec3f	vec3_new(float x, float y, float z)
{
	t_vec3f	vec;

	vec3_set(&vec, x, y, z);
	return (vec);
}
