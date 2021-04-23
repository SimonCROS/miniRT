#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_sphere(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	float		diametre;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !float_deserialize((char *)lst_get(data, 2), &diametre)
		|| !col_deserialize((char *)lst_get(data, 3), &color))
		return (NULL);
	diametre = fabsf(diametre);
	return (new_sphere(diametre, vec3_addv(pos, origin), color));
}

// static int	check(t_object *object, t_ray *ray, float tca, float thc)
// {
// 	float		t[2];

// 	t[0] = tca - thc;
// 	t[1] = tca + thc;
// 	if (t[0] > t[1])
// 	{
// 		t[0] = t[0] + t[1];
// 		t[1] = t[0] - t[1];
// 		t[0] = t[0] - t[1];
// 	}
// 	if (t[0] < 0)
// 	{
// 		t[0] = t[1];
// 		if (t[0] < 0)
// 			return (FALSE);
// 	}
// 	ray->length = t[0];
// 	ray->phit = vec3_addv(ray->origin, vec3_muld(ray->direction, ray->length));
// 	ray->nhit = vec3_normalize(vec3_subv(ray->phit, object->position));
// 	return (TRUE);
// }

static int	collides_sphere(t_object *object, t_ray *ray)
{
	// t_vector3	to_center;
	// float		tca;
	// float		d2;
	// float		radius2;
	// float		thc;

	// to_center = vec3_subv(object->position, ray->origin);
	// tca = vec3_dotv(to_center, ray->direction);
	// if (tca < 0)
	// 	return (0);
	// d2 = vec3_dotv(to_center, to_center) - tca * tca;
	// radius2 = object->data.sphere.radius * object->data.sphere.radius;
	// if (d2 > radius2)
	// 	return (0);
	// thc = sqrt(radius2 - d2);
	// return (check(object, ray, tca, thc));
	ray->length = resolve_quad(&object->data.sphere.quadric, ray->origin, ray->direction);
	if (!ray->length)
		return (FALSE);
	ray->phit = vec3_addv(ray->origin, vec3_muld(ray->direction, ray->length));
	ray->nhit = vec3_new(0, 1, 0);
	return (TRUE);
}

t_object	*new_sphere(float diametre, t_vector3 p, t_color color)
{
	t_object	*object;
	t_sphere	*sphere;

	object = new_default_object(p, vec3_new(0, 1, 0), color,
			(t_bipre)collides_sphere);
	if (!object)
		return (NULL);
	sphere = &object->data.sphere;
	sphere->radius = diametre * 0.5;
	sphere->quadric = (t_quadric){
		.a = 1,
		.b = 1,
		.c = 1,
		.g = -2 * p.x,
		.h = -2 * p.y,
		.i = -2 * p.z,
		.j = pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2) + pow(-sphere->radius, 2)
	};
	return (object);
}
