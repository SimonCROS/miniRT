#include <math.h>

#include "renderer.h"
#include "object.h"

static int	in_light(t_scene *scene, t_object *object, t_ray *light_ray,
	float length2)
{
	t_iterator	obj_iterator;
	t_object	*obj_test;

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
		if (length2 < 0)
			continue ;
		if (!camera->shadows || in_light(scene, object, &light_ray, length2))
			ray->color = color_add(ray->color,
					light->calculate_color(light, object, ray, &light_ray));
	}
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

static void	render_pixel(t_thread_data *data, t_scene *scene, size_t x,
	size_t y)
{
	t_ray		ray;
	t_object	*object;

	ray = compute_ray(scene->render, data->camera, x, y);
	object = nearest_object(scene->objects, &ray);
	if (object)
	{
		render_light(scene, data->camera, object, &ray);
		// y += (int)((double)10 * sin((double)x / (double)10));
		// y = fminf(y, fmaxf(0, scene->render->height - 1));
		data->vars->set_pixel(data->camera->render, x, y, ray.color);
	}
}

static void	render_chunk(t_thread_data *data, size_t start_x, size_t start_y)
{
	t_scene		*scene;
	size_t		x;
	size_t		y;

	scene = data->scene;
	x = start_x;
	while (x < start_x + scene->render->chunk_width && x < data->width)
	{
		y = start_y;
		while (y < start_y + scene->render->chunk_height && y < data->height)
		{
			render_pixel(data, scene, x, y);
			y++;
		}
		x++;
	}
}

void	*render_thread(t_thread_data *data, int *chunk)
{
	t_options	*params;
	int			chunk_x;
	int			chunk_y;
	int			ratio;

	params = data->scene->render;
	ratio = (int)ceilf(params->width / (float)params->chunk_width);
	chunk_x = *chunk % ratio * params->chunk_width;
	chunk_y = *chunk / ratio * params->chunk_height;
	render_chunk(data, chunk_x, chunk_y);
	if (log_msg(DEBUG, NULL))
	{
		printf("Rendering chunk (%d,%d)...", chunk_x, chunk_y);
		log_nl();
	}
	return (NULL);
}
