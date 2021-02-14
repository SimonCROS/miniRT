/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/02/12 15:24:41 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int		key_pressed(int i, t_vars *vars)
{
	if (i == 53)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		// TODO FREE ALL
		exit(0);
	}
	return (0);
}

void	free_light(void *p)
{
	t_light *light;
	
	light = (t_light*)p;
	free(light->position);
	free(light->color);
	free(light);
}

void	free_plan(void *p)
{
	t_object *plan;
	
	plan = (t_object*)p;
	free(plan);
}

t_vector3	camera_direction(t_vector3 dir, t_vector3 rot)
{
	static t_vector3	default_vector = (t_vector3) { 0, 0, -1 };
	t_vector3			axis;
	float				angle;

	axis = vec3_crossv(default_vector, vec3_normalize(rot));
	if (vec3_length_squared(axis) == 0)
		axis = (t_vector3) { 0, 1, 0 };
	angle = vec3_angle(default_vector, vec3_normalize(rot));
	return (vec3_rotate_axis(dir, vec3_normalize(axis), angle));
}

t_ray	compute_ray(t_camera *camera, float x, float y)
{
	t_ray ray;
	float ratio = WID / (float) HEI;
	float px = (2 * ((x + 0.5) / WID) - 1) * tan(FOV / 2 * M_PI / 180) * ratio;
	float py = (1 - 2 * ((y + 0.5) / HEI)) * tan(FOV / 2 * M_PI / 180);
	ray.direction = vec3_normalize(vec3_new(px, py, -1));
	ray.direction = vec3_normalize(camera_direction(ray.direction, *(camera->direction)));
	ray.color = color_new(0, 0, 0);
	ray.light = 1;
	ray.origin = camera->position;
	ray.length = INFINITY;
	return (ray);
}

int		render(t_vars *vars)
{
	static t_vector3 rot = (t_vector3) {0.2, 0, 1};
	static float cam_y_rot;
	static float cam_x_pos;
	static float lum_x_pos;

	t_data	img;
	img.img = mlx_new_image(vars->mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	// t_camera	*camera = new_camera(vec3_malloc(0, 0, 0), vec3_malloc(0, 0, -1), FOV);
	t_camera	*camera = new_camera(vec3_malloc(12, 20, -30), vec3_malloc(-1, -1, -1), FOV);

	t_list		*lights = ft_lst_new(&free_light);
	ft_lst_push(lights, new_light(0.5, vec3_malloc(0, 2, 10), color_clone(color_new(255, 255, 255))));
	ft_lst_push(lights, new_light(0.5, vec3_malloc(0, 2, -15), color_clone(color_new(255, 255, 255))));
	ft_lst_push(lights, new_light(0.5, vec3_malloc(0, 2, -40), color_clone(color_new(255, 255, 255))));
	ft_lst_push(lights, new_light(0.5, vec3_malloc(0, 2, -65), color_clone(color_new(255, 255, 255))));

	t_list		*plans = ft_lst_new(&free_plan);

	ft_lst_push(plans, new_plan(vec3_new(0, -5, -25), vec3_new(0, 1, 0), color_new(0, 0, 255)));

	// ft_lst_push(plans, new_triangle(vec3_new(-8, -5, -70), vec3_new(8, -5, -70), vec3_new(0, 8, -70), color_new(255, 255, 255)));

	// ft_lst_push(plans, new_sphere(6, vec3_new(-12, 10, -10), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(-12, 10, -20), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(-12, 10, -30), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(-12, 10, -40), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(-12, 10, -50), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(-12, 10, -60), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(-12, 10, -70), color_new(120, 36, 237)));

	// ft_lst_push(plans, new_sphere(6, vec3_new(12, 10, -10), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(12, 10, -20), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(12, 10, -30), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(12, 10, -40), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(12, 10, -50), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(12, 10, -60), color_new(120, 36, 237)));
	// ft_lst_push(plans, new_sphere(6, vec3_new(12, 10, -70), color_new(120, 36, 237)));

	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(-12, 2, -10), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(-12, 2, -20), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(-12, 2, -30), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(-12, 2, -40), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(-12, 2, -50), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(-12, 2, -60), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(-12, 2, -70), vec3_new(0, 1, 0), color_new(0, 150, 150)));

	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(12, 2, -10), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(12, 2, -20), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(12, 2, -30), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(12, 2, -40), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(12, 2, -50), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(12, 2, -60), vec3_new(0, 1, 0), color_new(0, 150, 150)));
	// ft_lst_push(plans, new_cylinder(4, 14, vec3_new(12, 2, -70), vec3_new(0, 1, 0), color_new(0, 150, 150)));

	// ft_lst_push(plans, new_square(70, vec3_new(8.5, -39, -35), vec3_new(1, 0, 0), color_new(36, 163, 237)));
	// ft_lst_push(plans, new_square(70, vec3_new(-8.5, -39, -35), vec3_new(1, 0, 0), color_new(36, 163, 237)));
	// ft_lst_push(plans, new_square(17.5, vec3_new(0, -12.5, -71), vec3_new(0, 0, 1), color_new(36, 163, 237)));

	for (size_t i = 0; i < WID; i++)
	{
		for (size_t j = 0; j < HEI; j++)
		{
			t_ray			ray = compute_ray(camera, i, j);
			t_object		*plan = NULL;
			t_iterator		*objectIterator = ft_iterator_new(plans);

			while (ft_iterator_has_next(objectIterator))
			{
				t_object *plan_test = ft_iterator_next(objectIterator);
				t_ray obj_ray = ray;

				if (collision(plan_test, &obj_ray))
				{
					if (obj_ray.length < ray.length)
					{
						ray = obj_ray;
						plan = plan_test;
					}
				}
			}
			free(objectIterator);

			if (!plan)
				continue;

			ray.color = color_mulf(plan->color, 0.2);

			t_iterator		*lightIterator = ft_iterator_new(lights);

			while (ft_iterator_has_next(lightIterator))
			{
				t_light *light = ft_iterator_next(lightIterator);

				t_vector3 lightDir = vec3_subv(*(light->position), ray.phit);
				float lightDistance2 = vec3_length_squared(lightDir);
				lightDir = vec3_normalize(lightDir);
				float LdotN = fmaxf(0, vec3_dotv(lightDir, ray.nhit));
				float tNearShadow = INFINITY;
				short inShadow = FALSE;

				objectIterator = ft_iterator_new(plans);

				t_ray light_ray;
				light_ray.origin = &(ray.phit);
				light_ray.direction = lightDir;
				light_ray.length = INFINITY;
				while (ft_iterator_has_next(objectIterator))
				{
					t_vector3 point;
					t_object *obj_test = ft_iterator_next(objectIterator);
					if (obj_test == plan)
						continue;
					if ((inShadow = collision(obj_test, &light_ray)))
					{
						if (light_ray.length * light_ray.length > lightDistance2)
							inShadow = 0;
						else
							break;
					}
				}
				free(objectIterator);
				float atm = (1 - inShadow) * light->brightness * LdotN;
				ray.color = color_add(ray.color, color_mul(plan->color, color_mulf(color_mulf(*(light->color), light->brightness), atm)));
			}
			set_pixel(&img, i, j, color_to_hex(ray.color));
		}
	}
	ft_lst_clear(plans);

	mlx_put_image_to_window(vars->mlx, vars->win, img.img, 0, 0);

	rot = vec3_rotate_y(rot, M_PI / (360 / 5));
	// cam_y_rot += 0.1;
	cam_x_pos += 0.1;
	// lum_x_pos += 2;

	free(plans);
	free(lights);
	free(camera);
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

	struct timeval stop, start;
	gettimeofday(&start, NULL);

	render(&vars);
	// mlx_loop_hook(vars.mlx, &render, &vars);
	mlx_key_hook(vars.win, &key_pressed, &vars);

	gettimeofday(&stop, NULL);
	printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
	printf("int           : %lu\nfloat         : %lu\nt_bipredicate : %lu\nt_vector3     : %lu\nt_color       : %lu\nt_square      : %lu\nu_data        : %lu\nt_object      : %lu\nt_ray         : %lu\n",
		sizeof(int),
		sizeof(float),
		sizeof(t_bipredicate),
		sizeof(t_vector3),
		sizeof(t_color),
		sizeof(t_square),
		sizeof(t_type),
		sizeof(t_object),
		sizeof(t_ray));

	mlx_loop(vars.mlx);
}
