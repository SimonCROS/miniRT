#include "minirt.h"
#include "object.h"

#include <math.h>

t_light	*parse_light(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	float		brightness;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !bounded_float_deserialize((char *)lst_get(data, 2), &brightness, 0,
			1)
		|| !col_deserialize((char *)lst_get(data, 3), &color)
		|| brightness < 0 || brightness > 1)
		return (NULL);
	return (new_light_point(brightness, vec3_addv(pos, origin), color));
}

static t_ray	calculate_ray(t_light *light, const t_ray *ray, float *length2)
{
	t_ray		light_ray;
	t_vector3	lightDir;
	float		light_distance2;

	lightDir = vec3_subv(light->position, ray->phit);
	light_distance2 = vec3_length_squared(lightDir);
	lightDir = vec3_normalize(lightDir);
	light_ray.origin = ray->phit;
	light_ray.direction = lightDir;
	light_ray.length = INFINITY;
	*length2 = light_distance2;
	return (light_ray);
}

static t_color	calculate_color(t_light *light, t_object *object,
	const t_ray *ray, const t_ray *light_ray)
{
	return (
		color_mul(object->color,
			color_mulf(light->color,
				light->brightness * light->brightness
				* fmaxf(0,
					vec3_dotv(light_ray->direction, ray->nhit)
				)
			)
		)
	);
}

t_light	*new_light_point(float brightness, t_vector3 position, t_color color)
{
	t_light	*light;

	light = new_default_light(position, color, brightness);
	if (!light)
		return (NULL);
	light->calculate_ray = calculate_ray;
	light->calculate_color = calculate_color;
	return (light);
}
