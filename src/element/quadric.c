#include "minirt.h"

#include "object.h"

int	collides_quadric(t_object *object, t_ray *ray)
{
	ray->length = resolve_quad(&object->data.quadric, ray->origin,
			ray->direction);
	if (!ray->length)
		return (FALSE);
	ray->phit = vec3_addv(ray->origin, vec3_muld(ray->direction, ray->length));
	ray->nhit = resolve_quad_norm(&object->data.quadric, ray->phit);
	return (TRUE);
}
