#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_cylinder(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	t_vector3	rot;
	float		attr[3];
	t_color		color;

	if (!args_size(lst_first(data), data->size, 6))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &(attr[0]))
		|| !float_deserialize((char *)lst_get(data, 4), &(attr[1]))
		|| !col_deserialize((char *)lst_get(data, 5), &color))
		return (NULL);
	attr[0] = fabsf(attr[0]) * 0.5;
	attr[1] = fabsf(attr[1]);
	return (new_cylinder(attr, vec3_addv(pos, origin), rot, color));
}

static int	intersect_side(t_object *obj, t_ray *ray)
{
	t_ray		to_bot;
	t_vector3	point;

	ray->phit = vec3_addv(vec3_muld(ray->direction, ray->length), ray->origin);
	to_bot.direction = obj->rotation;
	to_bot.origin = ray->phit;
	if (!intersect_plane(obj->data.cylinder.position2, obj->rotation, &to_bot))
		return (FALSE);
	to_bot.direction = vec3_negate(to_bot.direction);
	if (!intersect_plane(obj->position, obj->rotation, &to_bot))
		return (FALSE);
	point = vec3_addv(vec3_muld(obj->rotation, to_bot.length), obj->position);
	ray->nhit = vec3_normalize(vec3_subv(ray->phit, point));
	return (TRUE);
}

static int	collides_cylinder(t_object *obj, t_ray *ray)
{
	int			ret;

	ret = intersect_side(obj, ray);
	ret = collides_caps(obj, ray, obj->position, ret) || ret;
	ret = collides_caps(obj, ray, obj->data.cylinder.position2, ret) || ret;
	return (ret);
}

t_object	*new_cylinder(float *s, t_vector3 p, t_vector3 v, t_color color)
{
	t_object	*object;

	object = new_default_quadric(vec3_subv(p, vec3_muld(v, s[1] * 0.5)), v,
			color, (t_bipre)collides_cylinder);
	if (!object)
		return (NULL);
	object->data.cylinder.radius = s[0];
	object->data.cylinder.position2 = vec3_addv(p, vec3_muld(v, s[1] * 0.5));
	object->quadric = (t_quadric){
		.a = v.y * v.y + v.z * v.z,
		.b = v.x * v.x + v.z * v.z,
		.c = v.y * v.y + v.x * v.x,
		.d = -2 * v.x * v.y, .e = -2 * v.x * v.z, .f = -2 * v.y * v.z,
		.g = 2 * (v.z * (v.x * p.z - v.z * p.x) + v.y * (v.x * p.y - v.y * p.x)
			),
		.h = 2 * (v.x * (v.y * p.x - v.x * p.y) + v.z * (v.y * p.z - v.z * p.y)
			),
		.i = 2 * (v.x * (v.z * p.x - v.x * p.z) + v.y * (v.z * p.y - v.y * p.z)
			),
		.j = v.x * v.x * (p.y * p.y + p.z * p.z) + v.y * v.y * (p.x * p.x + p.z
			* p.z) + v.z * v.z * (p.x * p.x + p.y * p.y) - 2 * (v.x * v.y * p.x
			* p.y + v.x * v.z * p.x * p.z + v.y * v.z * p.y * p.z) - s[0] * s[0]
	};
	return (object);
}
