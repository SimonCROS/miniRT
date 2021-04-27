#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_cylinder(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	t_vector3	rot;
	float		attr[2];
	t_color		color;

	if (!args_size(lst_first(data), data->size, 6))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &(attr[0]))
		|| !float_deserialize((char *)lst_get(data, 4), &(attr[1]))
		|| !col_deserialize((char *)lst_get(data, 5), &color))
		return (NULL);
	attr[0] *= 0.5;
	return (new_cylinder(attr, vec3_addv(pos, origin), rot, color));
}

static int	intersect_base(t_vector3 pos, t_vector3 rot, t_ray *ray, float rad)
{
	t_vector3	v;
	float		d2;
	t_vector3	p;

	if (intersect_plane(pos, rot, ray))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, ray->origin);
		ray->phit = p;
		ray->nhit = rot;
		if (vec3_dotv(rot, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		v = vec3_subv(ray->phit, pos);
		d2 = vec3_length_squared(v);
		if (d2 <= rad * rad)
			return (TRUE);
	}
	return (FALSE);
}

static int	collides_caps(t_object *obj, t_ray *ray, t_vector3 base, int collides)
{
	t_ray		tmp;

	tmp = *ray;
	tmp.length = INFINITY;
	if (intersect_base(base, obj->rotation, &tmp, obj->data.cylinder.radius)
		&& (!collides || tmp.length < ray->length))
	{
		*ray = tmp;
		return (TRUE);
	}
	return (FALSE);
}

static int	collides_cylinder(t_object *obj, t_ray *ray)
{
	int			ret;

// TO patch
	ret = intersect_side(obj->position, obj->data.cylinder.position2,
			obj->rotation, ray);
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
