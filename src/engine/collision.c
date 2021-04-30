#include "minirt.h"
#include "object.h"

int	intersect_side(t_vector3 top, t_vector3 bot, t_vector3 rot, t_ray *ray)
{
	t_ray		to_bot;

	to_bot.direction = rot;
	to_bot.origin = ray->phit;
	if (!intersect_plane(bot, rot, &to_bot))
		return (FALSE);
	to_bot.direction = vec3_negate(to_bot.direction);
	if (!intersect_plane(top, rot, &to_bot))
		return (FALSE);
	return (TRUE);
}

int	collides_quadric(t_object *object, t_ray *ray)
{
	float	t[2];

	if (!resolve_quad_double(&object->quadric, ray->origin, ray->direction, t))
		return (FALSE);
	ray->length = t[0];
	ray->phit = vec3_addv(ray->origin, vec3_muld(ray->direction, ray->length));
	ray->nhit = resolve_quad_norm(&object->quadric, ray->phit);
	if (!object->collides)
		return (TRUE);
	if (object->collides(object, ray))
		return (TRUE);
	ray->length = t[1];
	if (!ray->length)
		return (FALSE);
	ray->phit = vec3_addv(ray->origin, vec3_muld(ray->direction, ray->length));
	ray->nhit = resolve_quad_norm(&object->quadric, ray->phit);
	return (object->collides(object, ray));
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
	return (FALSE);
}

int	collision(t_object *object, t_ray *ray)
{
	t_vector3	p;

	if (!object->is_plane && !object->is_quadric)
		return (object->collides(object, ray));
	else if (object->is_quadric)
		return (collides_quadric(object, ray));
	else if (intersect_plane(object->position, object->rotation, ray))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, ray->origin);
		ray->phit = p;
		ray->nhit = object->rotation;
		if (vec3_dotv(object->rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		if (!object->collides)
			return (TRUE);
		return (object->collides(object, ray));
	}
	return (FALSE);
}
