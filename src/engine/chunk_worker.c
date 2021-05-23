#include <math.h>

#include "renderer.h"
#include "object.h"

static t_color	render_ray(t_thread_data *data, t_vec2i pixel, int sample,
	int *changed)
{
	t_ray		ray;
	t_object	*object;

	ray = compute_ray(data->scene->render, data->camera,
			pixel.x + data->scene->render->samples_template[sample][0],
			pixel.y + data->scene->render->samples_template[sample][1]);
	object = nearest_object(data->scene->objects, &ray);
	if (object)
	{
		render_light(data->scene, data->camera, object, &ray);
		*changed = 1;
	}
	else
		ray.color = *(data->scene->background);
	return (ray.color);
}

static void	render_pixel(t_thread_data *data, t_scene *scene, t_vec2i pixel)
{
	int			i;
	int			changed;
	t_color		color;

	color = *(scene->background);
	changed = 0;
	i = 0;
	while (i < scene->render->samples)
	{
		if (i == 0)
			color = render_ray(data, pixel, i, &changed);
		else
			color = color_avg(color, render_ray(data, pixel, i, &changed));
		i++;
	}
	if (changed)
		data->vars->set_pixel(data->camera->render, pixel.x, pixel.y, color);
}

void	render_chunk(t_thread_data *data, int start_x, int start_y)
{
	t_scene	*scene;
	int		x;
	int		y;

	scene = data->scene;
	x = start_x;
	while (x < start_x + scene->render->chunk_width && x < data->width)
	{
		y = start_y;
		while (y < start_y + scene->render->chunk_height && y < data->height)
		{
			render_pixel(data, scene, (t_vec2i){x, y});
			y++;
		}
		x++;
	}
}
