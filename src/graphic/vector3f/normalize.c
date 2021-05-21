#include "graphics.h"

t_vec3f	vec3_normalize(t_vec3f v)
{
	float	length;

	length = vec3_length(v);
	if (length == 0)
		return (vec3_new(0, 0, 0));
	return (vec3_divd(v, length));
}
