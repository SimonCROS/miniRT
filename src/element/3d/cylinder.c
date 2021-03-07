#include "minirt.h"

static int	rot_deserialize(const char *str, t_vector3 *vector)
{
	t_vector3	rot;

	if (!vec3_deserialize(str, &rot))
		return (0);
	if (rot.x < -1 || rot.x > 1
		|| rot.y < -1 || rot.y > 1
		|| rot.z < -1 || rot.z > 1)
		return (0);
	*vector = rot;
	return (1);
}

t_object	*parse_cylinder(t_list *data)
{
	t_vector3	pos;
	t_vector3	rot;
	float		attr[2];
	t_color		color;
	int			e;

	if (data->size != 6)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && rot_deserialize((char *)lst_get(data, 2), &rot);
	e = e && ft_atof_full((char *)lst_get(data, 3), &(attr[0]));
	e = e && ft_atof_full((char *)lst_get(data, 4), &(attr[1]));
	e = e && color_deserialize((char *)lst_get(data, 5), &color);
	if (!e)
		return (NULL);
	attr[0] = fabsf(attr[0]);
	attr[1] = fabsf(attr[1]);
	return (new_cylinder(attr, pos, rot, color));
}

int	test_intersect(float t[2], float *current_z)
{
	*(current_z) = fminf(t[0], t[1]);
	return (*current_z > 0);
}

int	intersect_cylinder(t_object *cp, t_ray *r, float *current_z)
{
	t_vector3 eye = vec3_subv(r->origin, cp->position);
	float a = vec3_dotv(r->direction, r->direction) - pow(vec3_dotv(r->direction, cp->rotation), 2);
	float b = 2 * (vec3_dotv(r->direction, eye) - vec3_dotv(r->direction, cp->rotation) * vec3_dotv(eye, cp->rotation));
	float c = vec3_dotv(eye, eye) - pow(vec3_dotv(eye, cp->rotation), 2) - cp->data.cylinder.radius * cp->data.cylinder.radius;
	float t[2];
	float delta;
	delta = sqrt((b * b) - (4 * a * c));
	if (delta < 0)
		return (0);
	t[0] = (-b - (delta)) / (2 * a);
	t[1] = (-b + (delta)) / (2 * a);
	return (test_intersect(t, current_z));
}

int	intersect_side(t_object *object, t_ray *ray)
{
	t_ray	to_bot;

	ray->phit = vec3_addv(vec3_muld(ray->direction, ray->length), ray->origin);
	to_bot.direction = object->rotation;
	to_bot.origin = ray->phit;
	if (!intersect_plane(object->data.cylinder.position2, object->rotation, &to_bot))
		return 0;
	to_bot.direction = vec3_negate(to_bot.direction);
	if (!intersect_plane(object->position, object->rotation, &to_bot))
		return 0;
	t_vector3 point = vec3_addv(vec3_muld(object->rotation, to_bot.length), object->position);
	ray->nhit = vec3_normalize(vec3_subv(ray->phit, point));
	return 1;
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
			return (1);
	}
	return (0);
}

int	collides_cylinder(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*object;
	t_ray		tmp;
	int			collides;

	object = arg1;
	ray = arg2;
	collides = FALSE;
	if (!intersect_cylinder(object, ray, &(ray->length)))
		return (0);
	if (intersect_side(object, ray))
		collides = TRUE;
	tmp = *ray;
	tmp.length = INFINITY;
	if (intersect_base(object->position, object->rotation, &tmp, object->data.cylinder.radius))
	{
		if (!collides || tmp.length < ray->length)
		{
			*ray = tmp;
			collides = TRUE;
		}
	}
	tmp = *ray;
	tmp.length = INFINITY;
	if (intersect_base(object->data.cylinder.position2, object->rotation, &tmp, object->data.cylinder.radius))
		if (!collides || tmp.length < ray->length)
		{
			*ray = tmp;
			collides = TRUE;
		}
	return collides;
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
	object->data.cylinder.height = attrs[1];
	object->data.cylinder.position2 = pos2;
	return (object);
}
