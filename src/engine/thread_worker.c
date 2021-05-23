#include <math.h>

#include "renderer.h"

static void	*object_thread(t_thread_data *data, int *chunk)
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

static void	*triangle_thread(t_thread_data *data, int *chunk)
{
	t_options	*params;
	int			chunk_x;
	int			chunk_y;
	int			ratio;

	params = data->scene->render;
	ratio = (int)ceilf(params->width / (float)params->chunk_width);
	chunk_x = *chunk % ratio * params->chunk_width;
	chunk_y = *chunk / ratio * params->chunk_height;
	render_triangles(data->vars, bounding_box_from((t_vec2i){chunk_x, chunk_y},
			(t_vec2i){
			fminf(chunk_x + params->chunk_width, params->width),
			fminf(chunk_y + params->chunk_height, params->height)
		}));
	return (NULL);
}

void	*render_thread(t_thread_data *data, int *chunk)
{
	if (data->scene->triangles->size && !data->camera->show_triangles)
		triangle_thread(data, chunk);
	if (data->scene->objects->size)
		object_thread(data, chunk);
	return (NULL);
}
