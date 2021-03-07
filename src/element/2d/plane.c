#include <stdlib.h>
#include <math.h>

#include "convert.h"
#include "matrix.h"
#include "vector3.h"

#include "element/plan.h"

static int	rot_deserialize(const char *str, t_vector3 *vector)
{
	t_vector3	rot;

	if (!vec3_deserialize(str, &rot))
		return (0);
	if (rot.x < -1 || rot.x > 1 ||
		rot.y < -1 || rot.y > 1 ||
		rot.z < -1 || rot.z > 1)
		return (0);
	*vector = rot;
	return (1);
}

t_object	*parse_plane(t_list *data)
{
	t_vector3	pos;
	t_vector3	rot;
	t_color		color;
	int			e;

	if (data->size != 4)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && rot_deserialize((char *)lst_get(data, 2), &rot);
	e = e && color_deserialize((char *)lst_get(data, 3), &color);
	if (!e)
		return (NULL);
	return (new_plane(pos, rot, color));
}

t_object	*new_plane(t_vector3 pos, t_vector3 rot, t_color col)
{
	return (new_default_plane(pos, rot, col, NULL));
}

t_object	*new_default_plane(t_vector3 pos, t_vector3 rot, t_color col,
	t_bipredicate collides)
{
	t_object	*plan;

	plan = new_default_object(pos, rot, col, collides);
	if (!plan)
		return (NULL);
	plan->is_plane = 1;
	return (plan);
}

t_object	*new_default_object(t_vector3 pos, t_vector3 rot, t_color col,
	t_bipredicate collides)
{
	t_object	*object;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	object->position = pos;
	object->rotation = vec3_normalize(rot);
	object->collides = collides;
	object->color = col;
	object->is_plane = 0;
	return (object);
}

int	intersect_plane(t_vector3 pos, t_vector3 rot, t_ray *ray)
{
	float		dot;
	t_vector3	p0l0;

	dot = vec3_dotv(rot, ray->direction);
	if (dot != 0)
	{
		p0l0 = vec3_subv(pos, ray->origin);
		ray->length = vec3_dotv(p0l0, rot) / dot;
		return (ray->length >= 0);
	}
	return (0);
}

int	collision(t_object *object, t_ray *ray)
{
	t_vector3	p;

	if (!object->is_plane)
		return (object->collides(object, ray));
	else if (intersect_plane(object->position, object->rotation, ray))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, ray->origin);
		ray->phit = p;
		ray->nhit = object->rotation;
		if (vec3_dotv(object->rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		if (!object->collides)
			return (1);
		return (object->collides(object, ray));
	}
	return (0);
}