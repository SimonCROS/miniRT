#include <math.h>

#include "renderer.h"
#include "object.h"

#include "tpool.h"

void	render_triangles(t_vars *vars, t_bounding_box borders)
{
	t_iterator	obj_iterator;

	obj_iterator = iterator_new(vars->scene->triangles);
	while (iterator_has_next(&obj_iterator))
		project(vars, iterator_next(&obj_iterator), borders);
}

static void	render3(t_vars *vars, t_tpool *pool, t_thread_data *data,
	int *chunks)
{
	int	chunk;
	int	has_triangles;

	chunk = 0;
	has_triangles = data->scene->triangles->size;
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
	if (has_triangles && data->camera->show_triangles)
		render_triangles(vars, bounding_box_from((t_vec2i){},
				(t_vec2i){data->scene->render->width,
				data->scene->render->height}));
	free(chunks);
}

static void	fill_background(t_vars *vars, t_camera *camera, t_scene *scene)
{
	int		x;
	int		y;

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
	int				*chunks;

	params = scene->render;
	data.vars = vars;
	data.width = params->width;
	data.height = params->height;
	data.camera = camera;
	data.scene = scene;
	data.chunks = ceilf(params->width / (float)params->chunk_width)
		* ceilf(params->height / (float)params->chunk_height);
	if (camera->render == NULL) {
		camera->render = vars->init_image(vars, params);
	}
	pool = tpool_new(params->threads);
	if (camera->z_buffer == NULL) {
		camera->z_buffer = malloc(params->width * params->height * sizeof(float));
	}
	chunks = malloc(data.chunks * sizeof(int));
	if (!camera->z_buffer || !camera->render || !pool || !chunks)
	{
		perror("Error\nAn error occurred while starting rendering");
		exit_minirt(vars, pool, chunks, EXIT_FAILURE);
	}
	fill_z_buff(camera->z_buffer, params->width * params->height);
	fill_background(vars, camera, scene);
	render3(vars, pool, &data, chunks);
}

int	render(t_vars *vars)
{
	t_scene		*scene;
	t_camera	*camera;

	scene = vars->scene;
	camera = vars->camera;
	get_samples_template(scene->render->samples,
		scene->render->samples_template);
	render2(vars, camera, scene);
	vars->on_finished(vars, camera->render);
	log_msg(DEBUG, "Image successfully rendered");
	return (0);
}
