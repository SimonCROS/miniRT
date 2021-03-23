#include "minirt.h"
#include "object.h"

#include <math.h>

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
