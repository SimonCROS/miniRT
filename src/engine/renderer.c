#include <math.h>

#include "renderer.h"
#include "object.h"

static int	in_light(t_scene *scene, t_object *object, t_ray *light_ray,
	float length2)
{
	t_iterator	obj_iterator;
	t_object	*obj_test;

	light_ray->origin = vec3_addv(light_ray->origin,
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
		a = fmaxf(0, vec3_dotv(light_ray.direction, ray->nhit));
		if (length2 < 0 && a && (!camera->shadows
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

static void	render_pixel(t_thread_data *data, t_scene *scene, size_t x,
	size_t y)
{
	int			i;
	t_ray		ray;
	t_object	*object;
	int			changed;
	t_color		color;

	color = *(scene->background);
	changed = 0;
	i = 0;
	while (i < scene->render->samples)
	{
		ray = compute_ray(scene->render, data->camera,
				x + scene->render->samples_template[i][0],
				y + scene->render->samples_template[i][1]);
		object = nearest_object(scene->objects, &ray);
		if (object)
		{
			changed = 1;
			render_light(scene, data->camera, object, &ray);
			if (i == 0)
				color = ray.color;
			else
				color = color_avg(color, ray.color);
		}
		else
			color = color_avg(color, *(scene->background));
		i++;
	}
	if (changed)
		data->vars->set_pixel(data->camera->render, x, y, color);
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

void	*object_thread(t_thread_data *data, int *chunk)
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
