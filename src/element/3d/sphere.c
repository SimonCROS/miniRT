#include <stdlib.h>
#include <math.h>

#include "convert.h"
#include "matrix.h"
#include "vector3.h"

#include "element/plan.h"

t_object	*parse_sphere(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	float		diametre;
	t_color		color;
	int			e;

	if (data->size != 4)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && ft_atof_full((char *)lst_get(data, 2), &diametre);
	e = e && color_deserialize((char *)lst_get(data, 3), &color);
	if (!e)
		return (NULL);
	diametre = fabsf(diametre);
	return (new_sphere(diametre, vec3_addv(pos, origin), color));
}

int			collides_sphere(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*object;
	t_vector3	to_center;
	float		t0, t1;

	object = arg1;
	ray = arg2;

	to_center = vec3_subv(object->position, ray->origin);
	float tca = vec3_dotv(to_center, ray->direction);
	if (tca < 0) return 0;
	float d2 = vec3_dotv(to_center, to_center) - tca * tca;
	float radius2 = object->data.sphere.radius * object->data.sphere.radius;
	if (d2 > radius2) return 0;
	float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;
	if (t0 > t1)
	{
		t0 = t0 + t1;
		t1 = t0 - t1;
		t0 = t0 - t1;
	}
	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) return 0;
	}
	ray->length = t0;
	ray->phit = vec3_addv(ray->origin, vec3_muld(ray->direction, ray->length));
	ray->nhit = vec3_normalize(vec3_subv(ray->phit, object->position));
	return 1;
}

t_object		*new_sphere(float diametre, t_vector3 position, t_color color)
{
	t_object	*object;

	if (!(object = new_default_object(position, vec3_new(0, 1, 0), color, &collides_sphere)))
		return (NULL);
	object->data.sphere.radius = diametre / 2;
	return (object);
}
