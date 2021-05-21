#include "graphics.h"

t_vec3f	*vec3_set(t_vec3f *v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
	return (v);
}
