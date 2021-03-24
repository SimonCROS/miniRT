#include <math.h>
#include <pthread.h>

#include "mlx.h"

#include "libft.h"
#include "tpool.h"

#include "minirt.h"
#include "object.h"

static pthread_mutex_t	g_mutex_flush = PTHREAD_MUTEX_INITIALIZER;

void	render_light(t_scene *scene, t_object *object, t_light *light,
	t_ray *ray)
{
	t_iterator	objectIterator;
	t_ray		light_ray;
	t_vector3	lightDir;
	float		lightDistance2;
	float		LdotN;
	short		inShadow;
	float		atm;
	t_object	*obj_test;

	lightDir = vec3_subv(light->position, ray->phit);
	lightDistance2 = vec3_length_squared(lightDir);
	lightDir = vec3_normalize(lightDir);
	LdotN = fmaxf(0, vec3_dotv(lightDir, ray->nhit));
	inShadow = FALSE;
	light_ray.origin = ray->phit;
	light_ray.direction = lightDir;
	light_ray.length = INFINITY;
	objectIterator = iterator_new(scene->objects);
	while (iterator_has_next(&objectIterator))
	{
		obj_test = iterator_next(&objectIterator);
		if (obj_test == object)
			continue ;
		if (collision(obj_test, &light_ray)
			&& light_ray.length * light_ray.length <= lightDistance2)
			break ;
	}
	atm = (1 - inShadow) * light->brightness * LdotN;
	ray->color = color_add(ray->color, color_mul(object->color,
				color_mulf(color_mulf(light->color, light->brightness),
					atm)));
}

void	render_pixel_color(t_scene *scene, t_object *object, t_ray *ray)
{
	t_iterator	lightIterator;

	lightIterator = iterator_new(scene->lights);
	ray->color = color_mul(object->color, *(scene->ambiant));
	while (iterator_has_next(&lightIterator))
		render_light(scene, object, iterator_next(&lightIterator), ray);
}

void	render_pixel(t_thread_data *data, t_scene *scene, size_t x, size_t y)
{
	t_iterator	objectIterator;
	t_ray		ray;
	t_object	*object;
	t_object	*obj_test;
	t_ray		obj_ray;

	object = NULL;
	ray = compute_ray(scene->render, data->camera, x, y);
	objectIterator = iterator_new(scene->objects);
	while (iterator_has_next(&objectIterator))
	{
		obj_test = iterator_next(&objectIterator);
		obj_ray = ray;
		if (collision(obj_test, &obj_ray) && obj_ray.length < ray.length)
		{
			ray = obj_ray;
			object = obj_test;
		}
	}
	if (!object)
		ray.color = *(scene->background);
	else
		render_pixel_color(scene, object, &ray);
	data->vars->set_pixel(data->image, x, y, ray.color);
}

void	render_chunk(t_thread_data *data, size_t start_x, size_t start_y)
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
	pthread_mutex_lock(&g_mutex_flush);
	if (log_msg(DEBUG, NULL))
	{
		printf("Rendering chunk (%d,%d)...", chunk_x, chunk_y);
		log_nl();
	}
	data->vars->on_refresh(data->vars, data->image);
	pthread_mutex_unlock(&g_mutex_flush);
	return (NULL);
}

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
