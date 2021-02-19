/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/02/19 13:36:32 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "matrix.h"

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
	t_ray ray;
	float ratio = WID / (float) HEI;
	float px = (2 * ((x + 0.5) / WID) - 1) * tan(FOV / 2 * M_PI / 180) * ratio;
	float py = (1 - 2 * ((y + 0.5) / HEI)) * tan(FOV / 2 * M_PI / 180);
	ray.direction = vec3_normalize(vec3_new(px, py, 1));
	ray.direction = vec3_normalize(mat44_mul_vec(camera->c2w, ray.direction));
	ray.color = color_new(0, 0, 0);
	ray.light = 1;
	ray.origin = camera->position;
	ray.length = INFINITY;
	return (ray);
}

void	render_chunk(t_data *image, t_camera *camera, t_scene *scene, size_t chunk_x, size_t chunk_y)
{
	t_iterator		objectIterator = iterator_new(scene->objects);
	t_iterator		lightIterator = iterator_new(scene->lights);

	for (size_t x = 0; x < RENDER_WID; x++)
	{
		if (x + chunk_x >= WID)
			break;
		for (size_t y = 0; y < RENDER_HEI; y++)
		{
			if (y + chunk_y >= HEI)
				break;

			t_ray			ray = compute_ray(camera, x + chunk_x, y + chunk_y);
			t_object		*object = NULL;

			while (iterator_has_next(&objectIterator))
			{
				t_object *object_test = iterator_next(&objectIterator);
				t_ray obj_ray = ray;

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
			set_pixel(image, x + chunk_x, y + chunk_y, color_to_hex(ray.color));
		}
	}
}

// #define NUM_THREADS 5

int		render2(t_vars *vars, t_camera *camera, t_scene *scene)
{
	t_data	img;

	if (camera->render)
	{
		mlx_put_image_to_window(vars->mlx, vars->win, camera->render, 0, 0);
		return (0);
	}
	img.img = mlx_new_image(vars->mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	size_t i_iter = ceilf(WID / (float)RENDER_WID);
	size_t j_iter = ceilf(HEI / (float)RENDER_HEI);

	for (size_t j = 0; j < j_iter; j++)
	{
		size_t start_y = (RENDER_HEI) * j;

		for (size_t i = 0; i < i_iter; i++)
		{
			size_t start_x = (RENDER_WID) * i;
			render_chunk(&img, camera, scene, start_x, start_y);

			mlx_sync(MLX_SYNC_WIN_FLUSH_CMD, vars->win);
			mlx_put_image_to_window(vars->mlx, vars->win, img.img, 0, 0);
		}
	}
	camera->render = img.img;
	return (0);
}

t_scene	*get_scene(char *file)
{
	static t_scene *scene;

	if (!scene)
	{
		if (!(scene = malloc(sizeof(t_scene))))
			return (NULL);

t_list		*cameras = ft_lst_new(NULL);
ft_lst_push(cameras, new_camera(vec3_new(0, 0, 0), vec3_new(0, 0, 1), FOV));
ft_lst_push(cameras, new_camera(vec3_new(40, 30, 0), vec3_new(-1, -1, 1), FOV));
ft_lst_push(cameras, new_camera(vec3_new(12, 20, 90), vec3_new(-0.5, -0.6, -1), FOV));

t_list		*lights = ft_lst_new(&free);
ft_lst_push(lights, new_light(0.5, vec3_new(0, 2, -10), color_new(255, 255, 255)));
ft_lst_push(lights, new_light(0.5, vec3_new(0, 2, 15), color_new(255, 255, 255)));
ft_lst_push(lights, new_light(0.5, vec3_new(0, 2, 40), color_new(255, 255, 255)));
ft_lst_push(lights, new_light(0.5, vec3_new(0, 2, 65), color_new(255, 255, 255)));

t_list		*objects = ft_lst_new(&free);

ft_lst_push(objects, new_plan(vec3_new(0, -5, 25), vec3_new(0, 1, 0), color_new(0, 0, 255)));

ft_lst_push(objects, new_triangle(vec3_new(-8, -5, 70), vec3_new(8, -5, 70), vec3_new(0, 8, 70), color_new(255, 255, 255)));

ft_lst_push(objects, new_sphere(6, vec3_new(-12, 10, 10), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(-12, 10, 20), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(-12, 10, 30), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(-12, 10, 40), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(-12, 10, 50), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(-12, 10, 60), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(-12, 10, 70), color_new(120, 36, 237)));

ft_lst_push(objects, new_sphere(6, vec3_new(12, 10, 10), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(12, 10, 20), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(12, 10, 30), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(12, 10, 40), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(12, 10, 50), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(12, 10, 60), color_new(120, 36, 237)));
ft_lst_push(objects, new_sphere(6, vec3_new(12, 10, 70), color_new(120, 36, 237)));

ft_lst_push(objects, new_cylinder(4, 14, vec3_new(-12, 2, 10), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(-12, 2, 20), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(-12, 2, 30), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(-12, 2, 40), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(-12, 2, 50), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(-12, 2, 60), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(-12, 2, 70), vec3_new(0, 1, 0), color_new(0, 150, 150)));

ft_lst_push(objects, new_cylinder(4, 14, vec3_new(12, 2, 10), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(12, 2, 20), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(12, 2, 30), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(12, 2, 40), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(12, 2, 50), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(12, 2, 60), vec3_new(0, 1, 0), color_new(0, 150, 150)));
ft_lst_push(objects, new_cylinder(4, 14, vec3_new(12, 2, 70), vec3_new(0, 1, 0), color_new(0, 150, 150)));

ft_lst_push(objects, new_square(70, vec3_new(8.5, -39, 35), vec3_new(1, 0, 0), color_new(36, 163, 237)));
ft_lst_push(objects, new_square(70, vec3_new(-8.5, -39, 35), vec3_new(1, 0, 0), color_new(36, 163, 237)));
ft_lst_push(objects, new_square(17.5, vec3_new(0, -12.5, 71), vec3_new(0, 0, 1), color_new(36, 163, 237)));

		*scene = (t_scene) { cameras, lights, objects, 0 };
	}
	return (scene);
}

#include <time.h>

int		render(t_vars *vars, char *file)
{
	t_scene *scene;

	scene = get_scene(file);

	// TODO remove ^^
	clock_t begin = clock();

	int tmp = render2(vars, ft_lst_get(scene->cameras, scene->index), scene);

	// TODO remove ^^
	clock_t end = clock();
	printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	return (tmp);
}

int		on_key_pressed(int i, t_vars *vars)
{
	static int	started;
	t_scene		*scene;

	if (i == 53)
		return (on_close(vars));
	if (!started && i == 36)
	{
		started = 1;
		return (render(vars, "file.rt"));
	}
	if (started)
	{
		scene = get_scene(NULL);

		if (i == 123 || i == 125 || i == 124 || i == 126)
		{
			if (scene->cameras->size == 1)
				return (0);
			if (i == 123 || i == 125)
				scene->index--;
			if (i == 124 || i == 126)
				scene->index++;
			scene->index %= scene->cameras->size;
			if (scene->index < 0)
				scene->index = scene->cameras->size + scene->index;
			return (render(vars, "file.rt"));
		}
	}
	return (0);
}

#include <sys/time.h>

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
}
