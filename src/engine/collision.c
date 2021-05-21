#include "minirt.h"
#include "object.h"

#include <math.h>

int	intersect_side(t_vec3f top, t_vec3f bot, t_vec3f rot, t_ray *ray)
{
	t_ray		side_ray;

	side_ray.direction = rot;
	side_ray.origin = ray->phit;
	side_ray.length = INFINITY;
	if (!intersect_plane(bot, rot, &side_ray))
		return (FALSE);
	side_ray.direction = vec3_negate(side_ray.direction);
	if (!intersect_plane(top, rot, &side_ray))
		return (FALSE);
	return (TRUE);
}

int	collides_quadric(t_object *object, t_ray *ray)
{
	float	t[2];

	if (!resolve_quad_double(&object->quadric, ray->origin, ray->direction, t))
		return (FALSE);
	ray->length = t[0];
	ray->phit = vec3_add(ray->origin, vec3_muld(ray->direction, ray->length));
	ray->nhit = resolve_quad_norm(&object->quadric, ray->phit);
	if (!object->collides)
		return (TRUE);
	if (object->collides(object, ray))
		return (TRUE);
	ray->length = t[1];
	if (!ray->length)
		return (FALSE);
	ray->phit = vec3_add(ray->origin, vec3_muld(ray->direction, ray->length));
	ray->nhit = resolve_quad_norm(&object->quadric, ray->phit);
	return (object->collides(object, ray));
}

int	intersect_plane(t_vec3f pos, t_vec3f rot, t_ray *ray)
{
	float		dot;
	t_vec3f	p0l0;

	dot = vec3_dot(rot, ray->direction);
	if (dot != 0)
	{
		p0l0 = vec3_sub(pos, ray->origin);
		ray->length = vec3_dot(p0l0, rot) / dot;
		return (ray->length >= 0);
	}
	return (FALSE);
}

int	collision(t_object *object, t_ray *ray)
{
	t_vec3f	p;

	if (!object->is_plane && !object->is_quadric)
		return (object->collides(object, ray));
	else if (object->is_quadric)
		return (collides_quadric(object, ray));
	else if (intersect_plane(object->position, object->rotation, ray))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_add(p, ray->origin);
		ray->phit = p;
		ray->nhit = object->rotation;
		if (vec3_dot(object->rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		return (!object->collides || object->collides(object, ray));
	}
	return (FALSE);
}
