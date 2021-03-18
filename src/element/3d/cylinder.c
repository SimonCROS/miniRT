#include <stdlib.h>
#include <math.h>

#include "libft.h"

#include "element/plan.h"
#include "util/parsing.h"

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
	attr[0] = fabsf(attr[0]);
	attr[1] = fabsf(attr[1]);
	return (new_cylinder(attr, vec3_addv(pos, origin), rot, color));
}

int	test_intersect(float t[2], float *current_z)
{
	*(current_z) = fminf(t[0], t[1]);
	return (*current_z > 0);
}

int	intersect_cylinder(t_object *cp, t_ray *r, float *current_z)
{
	t_vector3	eye;
	float		a[3];
	float		t[2];
	float		delta;

	eye = vec3_subv(r->origin, cp->position);
	a[0] = vec3_dotv(r->direction, r->direction)
		- pow(vec3_dotv(r->direction, cp->rotation), 2);
	a[1] = 2 * (vec3_dotv(r->direction, eye) - vec3_dotv(r->direction,
				cp->rotation) * vec3_dotv(eye, cp->rotation));
	a[2] = vec3_dotv(eye, eye) - pow(vec3_dotv(eye, cp->rotation), 2)
		- cp->data.cylinder.radius * cp->data.cylinder.radius;
	delta = sqrt((a[1] * a[1]) - (4 * a[0] * a[2]));
	if (delta < 0)
		return (0);
	t[0] = (-a[1] - (delta)) / (2 * a[0]);
	t[1] = (-a[1] + (delta)) / (2 * a[0]);
	return (test_intersect(t, current_z));
}

int	intersect_side(t_object *obj, t_ray *ray)
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

int	intersect_base(t_vector3 position, t_vector3 rotation, t_ray *ray, float radius)
{
	t_vector3	v;
	float		d2;
	t_vector3	p;

	if (intersect_plane(position, rotation, ray))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, ray->origin);
		ray->phit = p;
		ray->nhit = rotation;
		if (vec3_dotv(rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		v = vec3_subv(ray->phit, position);
		d2 = vec3_length_squared(v);
		if (d2 <= radius * radius)
			return (TRUE);
	}
	return (FALSE);
}

int	collides_caps(t_object *obj, t_ray *ray, t_vector3 base, int collides)
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

int	collides_cylinder(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*obj;
	int			ret;

	obj = arg1;
	ray = arg2;
	if (!intersect_cylinder(obj, ray, &(ray->length)))
		return (FALSE);
	ret = intersect_side(obj, ray);
	ret = collides_caps(obj, ray, obj->position, ret) || ret;
	ret = collides_caps(obj, ray, obj->data.cylinder.position2, ret) || ret;
	return (ret);
}

t_object	*new_cylinder(float *attrs, t_vector3 pos, t_vector3 rot,
	t_color color)
{
	t_vector3	pos1;
	t_vector3	pos2;
	t_object	*object;

	pos1 = vec3_subv(pos, vec3_muld(rot, attrs[1] / 2));
	pos2 = vec3_addv(pos, vec3_muld(rot, attrs[1] / 2));
	object = new_default_object(pos1, rot, color, &collides_cylinder);
	if (!object)
		return (NULL);
	object->data.cylinder.radius = attrs[0] / 2;
	object->data.cylinder.position2 = pos2;
	return (object);
}
