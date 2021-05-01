#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_hyperboloid(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	float		attr[5];
	t_color		color;

	if (!args_size(lst_first(data), data->size, 8))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !float_deserialize((char *)lst_get(data, 2), &(attr[0]))
		|| !float_deserialize((char *)lst_get(data, 3), &(attr[1]))
		|| !float_deserialize((char *)lst_get(data, 4), &(attr[2]))
		|| !float_deserialize((char *)lst_get(data, 5), &(attr[3]))
		|| !float_deserialize((char *)lst_get(data, 6), &(attr[4]))
		|| !col_deserialize((char *)lst_get(data, 7), &color))
		return (NULL);
	attr[0] *= 0.5;
	return (new_hyperboloid(attr, vec3_addv(pos, origin), color));
}

static int	collides_hyperboloid(t_object *obj, t_ray *ray)
{
	return (intersect_side(obj->position, obj->data.hyperboloid.position2,
			obj->rotation, ray));
}

t_object	*new_hyperboloid(float *s, t_vector3 p, t_color color)
{
	t_object	*object;
	t_vector3	v;

	v = vec3_new(0, 1, 0);
	object = new_default_quadric(vec3_subv(p, vec3_muld(v, s[1] * 0.5)), v,
			color, (t_bipre)collides_hyperboloid);
	if (!object)
		return (NULL);
	object->data.cylinder.position2 = vec3_addv(p, vec3_muld(v, s[1] * 0.5));
	object->quadric = (t_quadric){
		.a = 1 / (s[2] * s[2]),
		.b = -1 / (s[3] * s[3]),
		.c = 1 / (s[4] * s[4]),
		.g = -2 * p.x / (s[2] * s[2]),
		.h = 2 * p.y / (s[3] * s[3]),
		.i = -2 * p.z / (s[4] * s[4]),
		.j = (p.x * p.x) / (s[2] * s[2]) - (p.y * p.y) / (s[3] * s[3])
		+ (p.z * p.z) / (s[4] * s[4]) - s[0] * s[0]
	};
	return (object);
}
