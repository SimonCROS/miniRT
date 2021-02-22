/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/02/22 15:31:21 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "matrix.h"
#include <pthread.h>

static int running;
static pthread_mutex_t mutex_flush		= PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_running	= PTHREAD_MUTEX_INITIALIZER;

void	set_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int		on_close(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	// TODO FREE ALL
	exit(0);
	return (0);
}

t_ray	compute_ray(t_camera *camera, float x, float y)
{
	static float ratio = WID / (float) HEI;
	static float hypo_len;

	if (!hypo_len)
		hypo_len = tan(FOV / 2 * M_PI / 180);
	t_ray ray;
	float px = (2 * ((x + 0.5) / WID) - 1) * hypo_len * ratio;
	float py = (1 - 2 * ((y + 0.5) / HEI)) * hypo_len;
	ray.direction = vec3_normalize(vec3_new(px, py, -1));
	ray.direction = vec3_normalize(mat44_mul_vec(camera->c2w, ray.direction));
	ray.color = color_new(0, 0, 0);
	ray.light = 1;
	ray.origin = camera->position;
	ray.length = INFINITY;
	return (ray);
}

void	render_chunk(t_thread_data *data, size_t chunk_x, size_t chunk_y)
{
	t_data			*image = &(data->image);
	t_iterator		objectIterator = iterator_new(data->scene->objects);
	t_iterator		lightIterator = iterator_new(data->scene->lights);

	for (size_t x = chunk_x; x < chunk_x + data->chunk_width; x++)
	{
		if (x >= WID)
			break;
		for (size_t y = chunk_y; y < chunk_y + data->chunk_height; y++)
		{
			if (y >= HEI)
				break;

			t_ray			ray = compute_ray(data->camera, x, y);
			t_object		*object = NULL;
			t_object		*object_test;
			t_ray			obj_ray;

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
			ray.color = color_mulf(object->color, 0.2);

			while (iterator_has_next(&lightIterator))
			{
				t_light *light = iterator_next(&lightIterator);

				t_vector3 lightDir = vec3_subv(light->position, ray.phit);
				float lightDistance2 = vec3_length_squared(lightDir); 
				lightDir = vec3_normalize(lightDir);
				float LdotN = fmaxf(0, vec3_dotv(lightDir, ray.nhit));
				float tNearShadow = INFINITY; 
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
			set_pixel(image, x, y, color_to_hex(ray.color));
		}
	}
}

void	force_put_image(t_vars *vars, t_data *image)
{
#if defined __APPLE__
	mlx_sync(MLX_SYNC_WIN_FLUSH_CMD, vars->win);
#elif defined __linux__
	*(int*)(vars->mlx + 80) = 1;
#endif
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
}

void	*render_thread(void *data)
{
	t_thread_data	*thread_data;
	size_t			i_iter;
	size_t			j_iter;

	pthread_mutex_lock(&mutex_running);
	running++;
	pthread_mutex_unlock(&mutex_running);
	thread_data = (t_thread_data*)data;
	i_iter = ceilf(thread_data->width / (float)thread_data->chunk_width);
	j_iter = ceilf(thread_data->height / (float)thread_data->chunk_height);

	for (size_t j = 0; j < j_iter; j++)
	{
		size_t start_y = thread_data->chunk_height * j + thread_data->y;

		for (size_t i = 0; i < i_iter; i++)
		{
			size_t start_x = thread_data->chunk_width * i + thread_data->x;
			render_chunk(thread_data, start_x, start_y);

			pthread_mutex_lock(&mutex_flush);
			force_put_image(thread_data->vars, &(thread_data->image));
			pthread_mutex_unlock(&mutex_flush);
		}
	}

	pthread_mutex_lock(&mutex_running);
	running--;
	pthread_mutex_unlock(&mutex_running);
	free(data);

	pthread_exit(NULL);
}

int		render2(t_vars *vars, t_camera *camera, t_scene *scene)
{

	static pthread_t		threads[NUM_THREADS];
	t_data					img;
	t_thread_data			thread_data;
	t_thread_data			*malloced_data;
	size_t					thread_line;
	size_t					thread_column;
	size_t					thread_width;
	size_t					thread_height;
	int						thread_id;

	if (camera->render)
	{
		mlx_put_image_to_window(vars->mlx, vars->win, camera->render, 0, 0);
		return (0);
	}
	img.img = mlx_new_image(vars->mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	thread_id = 0;
	thread_line = 4;
	thread_column = 2;
	thread_width = ceilf(WID / thread_line);
	thread_height = ceilf(HEI / thread_column);

	thread_data.vars = vars;
	thread_data.image = img;
	thread_data.width = thread_width;
	thread_data.height = thread_height;
	thread_data.camera = camera;
	thread_data.scene = scene;
	thread_data.chunk_width = fminf(CHUNK_WID, thread_width);
	thread_data.chunk_height = fminf(CHUNK_HEI, thread_height);
	for (size_t i = 0; i < thread_line; i++)
	{
		for (size_t j = 0; j < thread_column; j++)
		{
			thread_data.id = thread_id;
			thread_data.x = thread_width * i;
			thread_data.y = thread_height * j;
			if (!(malloced_data = malloc(sizeof(t_thread_data))))
				return (1);
			*malloced_data = thread_data;
			if (pthread_create(&threads[thread_id], NULL, render_thread, malloced_data))
			{
				printf("\33[31mUnable to create a thread.");
				free(malloced_data);
			}
			thread_id++;
		}
	}
	// while (1);
	
	camera->render = img.img;
	return (0);
}

# define O_1 color_new(123, 123, 125)

float newHeight() {
	static int max = 3;

	return (rand() / (float)RAND_MAX * (max - 1));
}

t_scene	*get_scene(char *file)
{
	static t_scene *scene;

	if (!scene)
	{
		if (!(scene = malloc(sizeof(t_scene))))
			return (NULL);

		t_list		*cameras = ft_lst_new(&free);
		ft_lst_push(cameras, new_camera(vec3_new(-30, 0, 25), vec3_new(0.9, 0, -1), FOV));
		ft_lst_push(cameras, new_camera(vec3_new(0, 0, 35), vec3_new(0, 0, -1), FOV));

		t_list		*lights = ft_lst_new(&free);
		ft_lst_push(lights, new_light(1, vec3_new(-40, 20, 50), color_new(255, 255, 255)));
		ft_lst_push(lights, new_light(0.5, vec3_new(0, 0, 30), color_new(255, 255, 255)));

		t_list		*objects = ft_lst_new(&free);
		ft_lst_push(objects, new_plan(vec3_new(0, 0, 0), vec3_new(0, 0, 1), O_1));

		float h = 4.33;
		float w = 5;
		float x = 0;
		float y = 0;
		for (x = -20; x < 20; x+=w)
		{
			for (y = -10; y < 10; y+=h)
			{
				float height = newHeight();

				ft_lst_push(objects, new_triangle(vec3_new(x - w / 2, y, height), vec3_new(x + w / 2, y, height), vec3_new(x, y - h, height), O_1));

				ft_lst_push(objects, new_triangle(vec3_new(x, y - h, height), vec3_new(x, y - h, 0), vec3_new(x + w / 2, y, 0), O_1));
				ft_lst_push(objects, new_triangle(vec3_new(x, y - h, height), vec3_new(x + w / 2, y, 0), vec3_new(x + w / 2, y, height), O_1));

				ft_lst_push(objects, new_triangle(vec3_new(x - w / 2, y, height), vec3_new(x - w / 2, y, 0), vec3_new(x + w / 2, y, 0), O_1));
				ft_lst_push(objects, new_triangle(vec3_new(x - w / 2, y, height), vec3_new(x + w / 2, y, 0), vec3_new(x + w / 2, y, height), O_1));

				ft_lst_push(objects, new_triangle(vec3_new(x - w / 2, y, height), vec3_new(x - w / 2, y, 0), vec3_new(x, y - h, 0), O_1));
				ft_lst_push(objects, new_triangle(vec3_new(x - w / 2, y, height), vec3_new(x, y - h, 0), vec3_new(x, y - h, height), O_1));

				height = newHeight();
				ft_lst_push(objects, new_triangle(vec3_new(x + w / 2, y, height), vec3_new(x, y - h, height), vec3_new(x + w, y - h, height), O_1));

				ft_lst_push(objects, new_triangle(vec3_new(x, y - h, height), vec3_new(x, y - h, 0), vec3_new(x + w / 2, y, 0), O_1));
				ft_lst_push(objects, new_triangle(vec3_new(x, y - h, height), vec3_new(x + w / 2, y, 0), vec3_new(x + w / 2, y, height), O_1));

				ft_lst_push(objects, new_triangle(vec3_new(x + w, y - h, height), vec3_new(x + w, y - h, 0), vec3_new(x + w / 2, y, 0), O_1));
				ft_lst_push(objects, new_triangle(vec3_new(x + w, y - h, height), vec3_new(x + w / 2, y, 0), vec3_new(x + w / 2, y, height), O_1));

				ft_lst_push(objects, new_triangle(vec3_new(x + w, y - h, height), vec3_new(x + w, y - h, 0), vec3_new(x, y - h, 0), O_1));
				ft_lst_push(objects, new_triangle(vec3_new(x + w, y - h, height), vec3_new(x, y - h, 0), vec3_new(x, y - h, height), O_1));
			}
		}

		*scene = (t_scene) { cameras, lights, objects, 0 };
	}
	return (scene);
}

#include <time.h>

int		render(t_vars *vars, char *file)
{
	t_scene *scene;

	scene = get_scene(file);
	return (render2(vars, ft_lst_get(scene->cameras, scene->index), scene));
}

int		on_key_pressed(int i, t_vars *vars)
{
	static int	started;
	t_scene		*scene;

	if (running)
		return (0);
	if (i == 53 || i == 65307)
		return (on_close(vars));
	if (!started && (i == 36 || i == 65293))
	{
		started = 1;
		return (render(vars, "file.rt"));
	}
	if (started)
	{
		scene = get_scene(NULL);

		if (i == 123 || i == 125 || i == 124 || i == 126 || i == 65361 || i == 65362 || i == 65363 || i == 65364)
		{
			if (scene->cameras->size == 1)
				return (0);
			if (i == 123 || i == 125 || i == 65361 || i == 65363)
				scene->index--;
			if (i == 124 || i == 126 || i == 65362 || i == 65364)
				scene->index++;
			scene->index %= scene->cameras->size;
			if (scene->index < 0)
				scene->index = scene->cameras->size + scene->index;
			return (render(vars, "file.rt"));
		}
	}
	return (0);
}

int		main(void)
{
	t_vars	vars;

	if (!(vars.mlx = mlx_init())) {
		printf("Error, can't generate the frame\n");
		exit(1);
	}

	printf("Launching\n");

	vars.win = mlx_new_window(vars.mlx, WID, HEI, "MiniRT - file.rt");

	mlx_hook(vars.win, 17, 0L, &on_close, &vars);
	mlx_key_hook(vars.win, &on_key_pressed, &vars);
	mlx_string_put(vars.mlx, vars.win, 0, 50, ~0, "Press enter to start");

	mlx_loop(vars.mlx);
	pthread_exit(NULL);
}
