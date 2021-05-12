#include <math.h>

#include "renderer.h"
#include "object.h"

#include "tpool.h"

static void	render_triangles(t_vars *vars, t_scene *scene, t_vector3 start,
	t_vector3 end)
{
	t_iterator	obj_iterator;

	obj_iterator = iterator_new(scene->triangles);
	while (iterator_has_next(&obj_iterator))
		project(vars, iterator_next(&obj_iterator), scene, start, end);
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
	render_triangles(data->vars, data->scene, vec3_new(chunk_x, chunk_y, 0),
		vec3_new(
			fminf(chunk_x + params->chunk_width, params->width),
			fminf(chunk_y + params->chunk_height, params->height), 0)
		);
	return (NULL);
}

static void	*render_thread(t_thread_data *data, int *chunk)
{
	if (data->scene->triangles->size && !data->camera->show_triangles)
		triangle_thread(data, chunk);
	if (data->scene->objects->size)
		object_thread(data, chunk);
	return (NULL);
}

static void	render3(t_vars *vars, t_tpool *pool, t_thread_data *data,
	size_t *chunks)
{
	size_t	chunk;
	int		has_triangles;
	int		has_objects;

	chunk = 0;
	has_triangles = data->scene->triangles->size;
	has_objects = data->scene->objects->size;
	if (has_triangles && data->camera->show_triangles)
		render_triangles(vars, data->scene,
			vec3_new(0, 0, 0),
			vec3_new(data->scene->render->width,
				data->scene->render->height, 0));
	while (chunk < data->chunks)
	{
		chunks[chunk] = chunk;
		if (!tpool_add_work(pool, (t_bifun)render_thread, data, chunks + chunk))
		{
			perror("Error\nAn error occurred while starting rendering");
			exit_minirt(vars, pool, chunks, EXIT_FAILURE);
		}
		chunk++;
	}
	tpool_set_name(pool, "CHUNK_WORKER");
	tpool_start(pool);
	tpool_wait(pool);
	tpool_destroy(pool);
	free(chunks);
}

static void	fill_background(t_vars *vars, t_camera *camera, t_scene *scene)
{
	size_t		x;
	size_t		y;

	x = 0;
	while (x < scene->render->width)
	{
		y = 0;
		while (y < scene->render->height)
			vars->set_pixel(camera->render, x, y++, *(scene->background));
		x++;
	}
}

static void	render2(t_vars *vars, t_camera *camera, t_scene *scene)
{
	t_tpool			*pool;
	t_thread_data	data;
	t_options		*params;
	size_t			*chunks;

	params = scene->render;
	data.vars = vars;
	data.width = params->width;
	data.height = params->height;
	data.camera = camera;
	data.scene = scene;
	camera->render = vars->init_image(vars, params);
	data.chunks = ceilf(params->width / (float)params->chunk_width)
		* ceilf(params->height / (float)params->chunk_height);
	pool = tpool_new(params->threads);
	camera->z_buffer = malloc(params->width * params->height * sizeof(float));
	chunks = malloc(data.chunks * sizeof(size_t));
	if (!camera->z_buffer || !camera->render || !pool || !chunks)
	{
		perror("Error\nAn error occurred while starting rendering");
		exit_minirt(vars, pool, chunks, EXIT_FAILURE);
	}
	fill_z_buff(camera->z_buffer, params->width * params->height);
	fill_background(vars, camera, scene);
	render3(vars, pool, &data, chunks);
	vars->on_finished(vars, camera->render);
}

int	render(t_vars *vars)
{
	t_scene		*scene;
	t_camera	*camera;

	scene = get_scene();
	camera = vars->camera;
	if (camera->render)
	{
		vars->on_finished(vars, camera->render);
		log_msg(DEBUG, "Image loaded from cache");
	}
	else
	{
		get_samples_template(scene->render->samples,
			scene->render->samples_template);
		render2(vars, camera, scene);
		log_msg(DEBUG, "Image successfully rendered");
	}
	return (0);
}
