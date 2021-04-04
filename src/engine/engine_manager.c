#include <math.h>

#include "renderer.h"
#include "object.h"

#include "tpool.h"

static void	load_triangles(t_vars *vars, t_camera *camera, t_scene *scene)
{
	t_iterator	obj_iterator;

	obj_iterator = iterator_new(scene->triangles);
	while (iterator_has_next(&obj_iterator))
		project(vars, iterator_next(&obj_iterator), scene, camera);
}

static void	render3(t_vars *vars, t_tpool *pool, t_thread_data *data,
	size_t *chunks)
{
	size_t			chunk;

	chunk = 0;
	load_triangles(vars, data->camera, data->scene);
	vars->on_refresh(vars, data->camera->render);
	pthread_mutex_init(&(data->mutex_flush), NULL);
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
		free(camera->z_buffer);
		perror("Error\nAn error occurred while starting rendering");
		exit_minirt(vars, pool, chunks, EXIT_FAILURE);
	}
	fill_z_buff(camera->z_buffer, params->width * params->height);
	render3(vars, pool, &data, chunks);
}

int	render(t_vars *vars)
{
	t_scene		*scene;
	t_camera	*camera;

	scene = get_scene();
	camera = lst_get(scene->cameras, scene->index);
	if (camera->render)
	{
		vars->on_refresh(vars, camera->render);
		log_msg(DEBUG, "Image loaded from cache");
	}
	else
	{
		render2(vars, camera, scene);
		vars->on_finished(vars, camera);
		log_msg(DEBUG, "Image successfully rendered");
	}
	return (0);
}
