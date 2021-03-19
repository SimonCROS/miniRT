#include "minirt.h"

// To remove
#include "element/plan.h"
#include "engine/ray.h"

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
