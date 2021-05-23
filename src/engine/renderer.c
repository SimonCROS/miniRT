#include <math.h>

#include "renderer.h"
#include "object.h"

static int	in_light(t_scene *scene, t_object *object, t_ray *light_ray,
	float length2)
{
	t_iterator	obj_iterator;
	t_object	*obj_test;

	light_ray->origin = vec3_add(light_ray->origin,
			vec3_muld(light_ray->direction, 0.001));
	obj_iterator = iterator_new(scene->objects);
	while (iterator_has_next(&obj_iterator))
	{
		obj_test = iterator_next(&obj_iterator);
		if (obj_test != object && collision(obj_test, light_ray)
			&& light_ray->length * light_ray->length <= length2)
			return (FALSE);
	}
	obj_iterator = iterator_new(scene->triangles);
	while (iterator_has_next(&obj_iterator))
	{
		obj_test = iterator_next(&obj_iterator);
		if (obj_test != object && collision(obj_test, light_ray)
			&& light_ray->length * light_ray->length <= length2)
			return (FALSE);
	}
	return (TRUE);
}

void	render_light(t_scene *scene, t_camera *camera, t_object *object,
	t_ray *ray)
{
	float		a;
	t_light		*light;
	t_ray		light_ray;
	t_iterator	lightIterator;
	float		length2;

	ray->color = color_mul(object->color, *(scene->ambiant));
	lightIterator = iterator_new(scene->lights);
	while (iterator_has_next(&lightIterator))
	{
		light = iterator_next(&lightIterator);
		light_ray = light->calculate_ray(light, ray, &length2);
		a = fmaxf(0, vec3_dot(light_ray.direction, ray->nhit));
		if (length2 >= 0 && a && (!camera->shadows
				|| in_light(scene, object, &light_ray, length2)))
		{
			if (camera->color_disruption)
				ray->color = color_add(ray->color, color_from_hsl(a, 1, .5));
			else
				ray->color = color_add(ray->color,
						light->calculate_color(light, object, ray, a));
		}
	}
	if (object->post_light_calculation)
		ray->color = object->post_light_calculation(camera, ray);
}

t_object	*nearest_object(t_list *objects, t_ray *ray)
{
	t_iterator	obj_iterator;
	t_object	*object;
	t_object	*obj_test;
	t_ray		obj_ray;

	object = NULL;
	obj_iterator = iterator_new(objects);
	while (iterator_has_next(&obj_iterator))
	{
		obj_test = iterator_next(&obj_iterator);
		obj_ray = *ray;
		if (collision(obj_test, &obj_ray) && obj_ray.length < ray->length)
		{
			*ray = obj_ray;
			object = obj_test;
		}
	}
	return (object);
}
