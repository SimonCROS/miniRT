#include "minirt.h"
#include "object.h"

#include <math.h>

t_light	*parse_light_laser(t_list *data, t_vec3f origin)
{
	t_vec3f	pos;
	t_vec3f	dir;
	float		attrs[2];
	t_color		color;

	if (!args_size(lst_first(data), data->size, 6))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &dir)
		|| !bounded_float_deserialize((char *)lst_get(data, 3), &attrs[0], 0, 1)
		|| !float_deserialize((char *)lst_get(data, 4), &attrs[1])
		|| !col_deserialize((char *)lst_get(data, 5), &color))
		return (NULL);
	attrs[1] *= 0.5;
	return (new_light_laser(attrs, vec3_add(pos, origin), dir, color));
}

static t_ray	calculate_ray(t_light *light, const t_ray *ray, float *length2)
{
	t_vec3f	p;
	t_ray		light_ray;
	float		inter_dist;

	*length2 = -1;
	light_ray.origin = ray->phit;
	light_ray.direction = vec3_negate(light->data.laser.direction);
	light_ray.length = INFINITY;
	if (!intersect_plane(light->position, light->data.laser.direction,
			&light_ray))
		return (light_ray);
	p = vec3_muld(light_ray.direction, light_ray.length);
	p = vec3_add(p, light_ray.origin);
	if (vec3_dot(light->data.laser.direction, light_ray.direction) > 0)
		light_ray.nhit = vec3_negate(light_ray.nhit);
	light_ray.phit = p;
	light_ray.nhit = light->data.laser.direction;
	inter_dist = vec3_distance_squared(light_ray.phit, light->position);
	if (inter_dist > light->data.laser.radius * light->data.laser.radius)
		return (light_ray);
	*length2 = light_ray.length * light_ray.length;
	return (light_ray);
}

t_light	*new_light_laser(float s[2], t_vec3f position, t_vec3f direction,
	t_color color)
{
	t_light	*light;

	light = new_default_light(position, color, s[0]);
	if (!light)
		return (NULL);
	light->data.laser.radius = s[1];
	light->data.laser.direction = direction;
	light->calculate_ray = calculate_ray;
	return (light);
}
