#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_sphere(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	float		diametre;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !float_deserialize((char *)lst_get(data, 2), &diametre)
		|| !col_deserialize((char *)lst_get(data, 3), &color))
		return (NULL);
	diametre = fabsf(diametre);
	return (new_sphere(diametre, vec3_addv(pos, origin), color));
}

t_object	*new_sphere(float diametre, t_vector3 p, t_color color)
{
	t_object	*object;

	object = new_default_quadric(color);
	if (!object)
		return (NULL);
	object->data.quadric = (t_quadric){
		.a = 1,
		.b = 1,
		.c = 1,
		.g = -2 * p.x,
		.h = -2 * p.y,
		.i = -2 * p.z,
		.j = pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2) - pow(diametre * 0.5, 2)
	};
	return (object);
}
