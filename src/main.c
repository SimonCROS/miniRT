/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/03/03 16:07:27 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "matrix.h"
#include "bitmap.h"
#include <pthread.h>
#include <stdio.h>
#if defined __linux__
# include <X11/Xlib.h>
#endif

static pthread_mutex_t g_mutex_flush	= PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t g_mutex_running	= PTHREAD_MUTEX_INITIALIZER;

/*
** If request == 0, reset the current thread id, set the chunk_count, and
**  returns -1
** If request == 1, returns the id of the next chunk, or reset to default values
**  there is no other chunk
** Else, returns the id of the current chunk
*/
int		next_chunk(int request, int chunk_count)
{
	static int	id = -1;
	static int	count = -1;

	if (request == 0)
	{
		id = -1;
		count = chunk_count;
	}
	// TODO Dangerous stopping, the end is reached, a thread is rendering and a key is pressed
	else if (request == 1)
		if (++id < count)
			return (id);
		else
		{
			id = -1;
			count = -1;
		}
	else if (request == 2)
		return (id);
	return (id);
}

void	mlx_set_pixel(t_image *data, int x, int y, t_color color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color_to_hex(color);
}

int		on_close(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	// TODO FREE ALL
	exit(0);
	return (0);
}

t_ray	compute_ray(t_render_params *render, t_camera *camera, float x, float y)
{
	static float ratio;
	static float hypo_len;
	t_ray ray;
	float px;
	float py;

	if (!ratio)
		ratio = render->width / (float) render->height;
	if (!hypo_len)
		hypo_len = tan(camera->fov / 2 * M_PI / 180);
	px = (2 * ((x + 0.5) / render->width) - 1) * hypo_len * ratio;
	py = (1 - 2 * ((y + 0.5) / render->height)) * hypo_len;
	ray.direction = vec3_normalize(vec3_new(px, py, -1));
	ray.direction = vec3_normalize(mat44_mul_vec(camera->c2w, ray.direction));
	ray.color = color_new(0, 0, 0);
	ray.light = 1;
	ray.origin = camera->position;
	ray.length = INFINITY;
	return (ray);
}

void	render_chunk(t_thread_data *data, int start_x, int start_y)
{
	t_iterator		objectIterator = iterator_new(data->scene->objects);
	t_iterator		lightIterator = iterator_new(data->scene->lights);

	for (size_t x = start_x; x < start_x + data->scene->render->chunk_width; x++)
	{
		if (x >= data->width)
			break;
		for (size_t y = start_y; y < start_y + data->scene->render->chunk_height; y++)
		{
			if (y >= data->height)
				break;

			t_ray	ray = compute_ray(data->scene->render, data->camera, x, y);
			t_object	*object = NULL;
			t_object	*object_test;
			t_ray	obj_ray;

			while (iterator_has_next(&objectIterator))
			{
				object_test = iterator_next(&objectIterator);
				obj_ray = ray;

				if (collision(object_test, &obj_ray))
				{
					if (obj_ray.length < ray.length)
					{
						ray = obj_ray;
						object = object_test;
					}
				}
			}

			iterator_reset(&objectIterator);

			if (!object)
				continue;
			ray.color = color_mul(object->color, *(data->scene->ambiant));

			while (iterator_has_next(&lightIterator))
			{
				t_light *light = iterator_next(&lightIterator);

				t_vector3 lightDir = vec3_subv(light->position, ray.phit);
				float lightDistance2 = vec3_length_squared(lightDir); 
				lightDir = vec3_normalize(lightDir);
				float LdotN = fmaxf(0, vec3_dotv(lightDir, ray.nhit));
				short inShadow = FALSE;

				t_ray light_ray;
				light_ray.origin = ray.phit;
				light_ray.direction = lightDir;
				light_ray.length = INFINITY;
				while (iterator_has_next(&objectIterator))
				{
					t_vector3 point;
					t_object *obj_test = iterator_next(&objectIterator);
					if (obj_test == object)
						continue;
					if ((inShadow = collision(obj_test, &light_ray)))
					{
						if (light_ray.length * light_ray.length > lightDistance2)
							inShadow = 0;
						else
							break;
					}
				}
				iterator_reset(&objectIterator);
				float atm = (1 - inShadow) * light->brightness * LdotN;
				ray.color = color_add(ray.color, color_mul(object->color, color_mulf(color_mulf(light->color, light->brightness), atm)));
			}
			iterator_reset(&lightIterator);
			data->vars->set_pixel(data->image, x, y, ray.color);
		}
	}
}

void	force_put_image(t_vars *vars, t_image *image)
{
#if defined __APPLE__
	mlx_sync(MLX_SYNC_WIN_FLUSH_CMD, vars->win);
#elif defined __linux__
	*(int*)(vars->mlx + 80) = 1;
#endif
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
}

void	*render_thread(t_thread_data *data)
{
	t_render_params	*params;
	int				chunk_id;
	int				chunk_x;
	int				chunk_y;
	int				ratio;

	params = data->scene->render;
	ratio = (int)ceilf(params->width / (float)params->chunk_width);
	while (1)
	{
		pthread_mutex_lock(&g_mutex_running);
		chunk_id = next_chunk(1, 0);
		pthread_mutex_unlock(&g_mutex_running);
		if (chunk_id == -1)
			break ;
		chunk_x = chunk_id % ratio * params->chunk_width;
		chunk_y = chunk_id / ratio * params->chunk_height;
		render_chunk(data, chunk_x, chunk_y);
		pthread_mutex_lock(&g_mutex_flush);
		if (data->vars->on_refresh)
			data->vars->on_refresh(data->vars, data->image);
		pthread_mutex_unlock(&g_mutex_flush);
	}
	free(data);
	pthread_exit(NULL);
}

t_image	*mlx_init_image(t_vars *vars, t_render_params *params)
{
	t_image *img;

	img = malloc(sizeof(t_image));
	if (!img)
		return (NULL);
	img->img = mlx_new_image(vars->mlx, params->width, params->height);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
	return (img);
}

t_bitmap	*bmp_init_image(t_vars *vars, t_render_params *params)
{
	return (bmp_init(params->width, params->height));
}

void	mlx_finished(t_camera *camera, t_image *image)
{
	camera->render = image->img;
	free(image);
}

void	bmp_finished(t_camera *camera, t_bitmap *image)
{
	bmp_save("render.bmp", image);
	free(image);
}

int		render2(t_vars *vars, t_camera *camera, t_scene *scene)
{
	static pthread_t		threads[MAX_THREADS];
	t_thread_data			data;
	t_thread_data			*malloced_data;
	t_render_params			*params;
	int						thread_id;

	params = scene->render;
	thread_id = 0;
	data.vars = vars;
	data.image = vars->init_image(vars, params);
	data.width = params->width;
	data.height = params->height;
	data.camera = camera;
	data.scene = scene;
	data.chunks = ceilf(params->width / (float)params->chunk_width) * ceilf(params->height / (float)params->chunk_height);
	next_chunk(0, data.chunks);
	
	while (thread_id < params->threads)
	{
		if (!(malloced_data = malloc(sizeof(t_thread_data))))
			return (1);
		*malloced_data = data;
		data.id = thread_id;
		if (pthread_create(&threads[thread_id], NULL, (t_fun)render_thread, malloced_data))
		{
			printf("\33[31mUnable to create a thread.");
			free(malloced_data);
			break ;
		}
		thread_id++;
	}
	thread_id = 0;
	while (thread_id < params->threads)
	{
		pthread_join(threads[thread_id], NULL);
		thread_id++;
	}
	vars->on_finished(camera, data.image);
	return (0);
}

t_scene	*get_scene(char *file)
{
	static t_scene *scene;

	if (!scene)
	{
		scene = parse_file(file);
		if (!scene)
			return (NULL);
	}
	return (scene);
}

int		render(t_vars *vars)
{
	t_scene		*scene;
	t_camera	*camera;

	scene = get_scene(NULL);
	camera = lst_get(scene->cameras, scene->index);
	if (camera->render && vars->on_refresh)
	{
		vars->on_refresh(vars, camera->render);
		return (0);
	}
	return (render2(vars, camera, scene));
}

int		on_key_pressed(int i, t_vars *vars)
{
	static int	started;
	t_scene		*scene;

	if (next_chunk(2, 0) != -1)
		return (0);
	if (i == 53 || i == 65307)
		return (on_close(vars));
	if (!started)
	{
		started = 1;
		return (render(vars));
	}
	if (started)
	{
		scene = get_scene(NULL);

		if (i == 123 || i == 125 || i == 124 || i == 126 || i == 65361 || i == 65362 || i == 65363 || i == 65364)
		{
			if (scene->cameras->size == 1)
				return (0);
			if (i == 123 || i == 125 || i == 65361 || i == 65364)
				scene->index--;
			if (i == 124 || i == 126 || i == 65362 || i == 65363)
				scene->index++;
			scene->index %= scene->cameras->size;
			if (scene->index < 0)
				scene->index = scene->cameras->size + scene->index;
			return (render(vars));
		}
	}
	return (0);
}

void	load_frame(char *file, t_scene *scene)
{
	char	*name;
	t_vars	vars;

	#if defined __linux__
		XInitThreads();
	#endif
	if (!(vars.mlx = mlx_init())) {
		printf("Error, can't generate the frame\n");
		exit(1);
	}
	name = ft_strjoin("MiniRT - ", file);
	if (!name)
		return ; // TODO
	vars.win = mlx_new_window(vars.mlx, scene->render->width, scene->render->height, name);
	vars.init_image = (t_bifun)mlx_init_image;
	vars.set_pixel = (t_pixel_writer)mlx_set_pixel;
	vars.on_refresh = (t_bicon)force_put_image;
	vars.on_finished = (t_bicon)mlx_finished;
	free(name);
	mlx_hook(vars.win, 17, 0L, &on_close, &vars);
	mlx_key_hook(vars.win, &on_key_pressed, &vars);
	mlx_string_put(vars.mlx, vars.win, 0, 50, ~0, "Press any key to start");

	mlx_loop(vars.mlx);
}

void	load_image(char *file, t_scene *scene)
{
	t_bitmap	*image;
	t_vars		vars;

	vars.init_image = (t_bifun)bmp_init_image;
	vars.set_pixel = (t_pixel_writer)bmp_set_pixel;
	vars.on_refresh = (NULL);
	vars.on_finished = (t_bicon)bmp_finished;
	render(&vars);
}

int main(int argc, char **argv)
{
	t_scene	*scene;
	int		save;

	printf("Launching\n");

	save = 0;
	if (argc != 2)
	{
		if (argc == 3 && !ft_strcmp(argv[2], "--save"))
			save = 1;
		else
			return (0);
	}

	scene = get_scene(argv[1]);
	if (save)
		load_image(argv[1], scene);
	else
		load_frame(argv[1], scene);
	pthread_exit(NULL);
}
