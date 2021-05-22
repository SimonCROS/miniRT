#include "minirt.h"
#include "object.h"

#include <math.h>

t_light	*parse_light_point(t_list *data, t_vec3f origin)
{
	t_vec3f	pos;
	float	bright;
	t_color	color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !bounded_float_deserialize((char *)lst_get(data, 2), &bright, 0, 1)
		|| !col_deserialize((char *)lst_get(data, 3), &color))
		return (NULL);
	return (new_light_point(bright, vec3_add(pos, origin), color));
}

static t_ray	calculate_ray(t_light *light, const t_ray *ray, float *length2)
{
	t_ray	light_ray;
	t_vec3f	lightDir;
	float	light_distance2;

	lightDir = vec3_sub(light->position, ray->phit);
	light_distance2 = vec3_length_squared(lightDir);
	lightDir = vec3_normalize(lightDir);
	light_ray.origin = ray->phit;
	light_ray.direction = lightDir;
	light_ray.length = INFINITY;
	*length2 = light_distance2;
	return (light_ray);
}

t_light	*new_light_point(float brightness, t_vec3f position, t_color color)
{
	t_light	*light;

	light = new_default_light(position, color, brightness);
	if (!light)
		return (NULL);
	light->calculate_ray = calculate_ray;
	return (light);
}
