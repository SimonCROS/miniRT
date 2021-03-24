#include <math.h>
#include "mlx.h"

#include "libft.h"
#include "tpool.h"

#include "minirt.h"
#include "renderer.h"
#include "object.h"

t_bitmap	*bmp_init_image(t_vars *vars, t_options *params)
{
	(void)vars;
	return (bmp_init(params->width, params->height));
}

// void	bmp_free(t_vars *vars)
// {
// 	free(image->body);
// 	free(image);
// 	free_scene(get_scene());
// }

// void	bmp_exit(t_vars *vars)
// {
// 	free(image->body);
// 	free(image);
// 	free_scene(get_scene());
// }

void	bmp_finished(t_vars *vars, t_camera *camera)
{
	t_bitmap	*image;

	(void)vars;
	image = camera->render;
	log_msg(INFO, "Saving...");
	(void)camera;
	if (!bmp_save("render.bmp", image))
	{
		log_msg(DEBUG, "Error while saving image to bmp.");
		perror("Error\nError while saving image to bmp");
	}
	free(image->body);
	free(image);
	free_scene(get_scene());
}

int	render2(t_vars *vars, t_camera *camera, t_scene *scene)
{
	t_tpool			*pool;
	t_thread_data	data;
	t_options		*params;
	size_t			*chunks;
	size_t			chunk;

	params = scene->render;
	data.vars = vars;
	data.image = vars->init_image(vars, params);
	// if (!data.image)
	// 	return ()
	data.width = params->width;
	data.height = params->height;
	data.camera = camera;
	data.scene = scene;
	pthread_mutex_init(&(data.mutex_flush), NULL);
	data.chunks = ceilf(params->width / (float)params->chunk_width)
		* ceilf(params->height / (float)params->chunk_height);
	pool = tpool_new(params->threads);
	chunks = malloc(data.chunks * sizeof(size_t));
	if (!chunks)
		return (FALSE);
	chunk = 0;
	vars->on_refresh(vars, data.image);
	while (chunk < data.chunks)
	{
		chunks[chunk] = chunk;
		tpool_add_work(pool, (t_bifun)render_thread, &data, chunks + chunk);
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
	return (TRUE);
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
		return (0);
	}
	return (render2(vars, camera, scene));
}

void	load_image(char *file, t_scene *scene)
{
	t_vars		vars;

	(void)file;
	(void)scene;
	vars.init_image = (t_bifun)bmp_init_image;
	vars.set_pixel = (t_pixel_writer)bmp_set_pixel;
	vars.on_refresh = null_biconsumer();
	vars.on_finished = (t_bicon)bmp_finished;
	render(&vars);
}

int	main(int argc, char **argv)
{
	int		save;

	pthread_setname_ft("MAIN");
	log_msg(INFO, "Starting program...");
	save = 0;
	if (argc == 3 && !ft_strcmp(argv[2], "--save"))
		save = 1;
	else if (argc != 2)
	{
		errno = EINVAL;
		perror("Error\nUnable to parse program arguments");
		exit(EXIT_FAILURE);
	}
	if (!load_scene(argv[1]))
	{
		log_msg(FATAL, "An error occurred while parsing the file");
		perror("Error\nAn error occurred while parsing the file");
		exit(EXIT_FAILURE);
	}
	log_msg(INFO, "Parsing finished");
	if (save)
		load_image(argv[1], get_scene());
	else
		init_window(argv[1], get_scene());
	exit(EXIT_SUCCESS);
}
