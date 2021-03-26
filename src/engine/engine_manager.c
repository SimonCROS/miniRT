#include <math.h>

#include "renderer.h"
#include "object.h"

#include "tpool.h"

static void	render2(t_vars *vars, t_camera *camera, t_scene *scene)
{
	t_tpool			*pool;
	t_thread_data	data;
	t_options		*params;
	size_t			*chunks;
	size_t			chunk;

	params = scene->render;
	data.vars = vars;
	data.width = params->width;
	data.height = params->height;
	data.camera = camera;
	data.scene = scene;
	data.image = vars->init_image(vars, params);
	data.chunks = ceilf(params->width / (float)params->chunk_width)
		* ceilf(params->height / (float)params->chunk_height);
	pool = tpool_new(params->threads);
	chunks = malloc(data.chunks * sizeof(size_t));
	if (!data.image || !pool || !chunks)
	{
		perror("Error\nAn error occurred while starting rendering");
		exit_minirt(vars, pool, chunks, EXIT_FAILURE);
	}
	chunk = 0;
	vars->on_refresh(vars, data.image);
	pthread_mutex_init(&(data.mutex_flush), NULL);
	while (chunk < data.chunks)
	{
		chunks[chunk] = chunk;
		if (!tpool_add_work(pool, (t_bifun)render_thread,
				&data, chunks + chunk))
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
	log_msg(INFO, "Image successfully rendered");
	camera->render = data.image;
	vars->on_finished(vars, camera);
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
		render2(vars, camera, scene);
	return (0);
}
