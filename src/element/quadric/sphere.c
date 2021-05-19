#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_sphere(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	float		diametre;
	float		radius;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !float_deserialize((char *)lst_get(data, 2), &diametre)
		|| !col_deserialize((char *)lst_get(data, 3), &color))
		return (NULL);
	radius = fabsf(diametre) * 0.5;
	return (new_sphere(radius, vec3_addv(pos, origin), color));
}

static t_color	calculate_sphere_color(const t_camera *cam, const t_ray *ray)
{
	float	u;
	float	v;
	int		i;
	int		j;
	t_color	color;

	color = ray->color;
	if (cam->sphere_up_map)
	{
		color = color_new(200, 200, 200);
		u = 1 - (atan2f(ray->nhit.z, ray->nhit.x) + M_PI) / (2 * M_PI);
		v = (asinf(ray->nhit.y) + M_PI / 2) / M_PI;
		i = u * 100;
		j = v * 100;
		if ((i % 10 < 5 && j % 10 >= 5) || (i % 10 >= 5 && j % 10 < 5))
			color = color_new(125, 125, 125);
	}	
	return (color);
}

t_object	*new_sphere(float radius, t_vector3 p, t_color color)
{
	t_object	*object;

	object = new_default_quadric(p, vec3_new(0, 1, 0), color, NULL);
	if (!object)
		return (NULL);
	object->quadric = (t_quadric){
		.a = 1,
		.b = 1,
		.c = 1,
		.g = -2 * p.x,
		.h = -2 * p.y,
		.i = -2 * p.z,
		.j = p.x * p.x + p.y * p.y + p.z * p.z - radius * radius
	};
	object->post_light_calculation = calculate_sphere_color;
	return (object);
}
