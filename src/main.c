/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/02/01 15:58:32 by scros            ###   ########lyon.fr   */
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
	ft_putnbr_fd(i, 1);
	ft_putendl_fd("", 1);
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
	t_plan *plan;
	
	plan = (t_plan*)p;
	free(plan->position);
	free(plan->rotation);
	free(plan->color);
	free(plan);
}

void	free_sphere(void *p)
{
	t_sphere *sphere;
	
	sphere = (t_sphere*)p;
	free(sphere->position);
	free(sphere->color);
	free(sphere);
}

t_vector3	compute_ray(t_camera *camera, float x, float y)
{
	float ratio = WID / (float) HEI;
	float px = (2 * ((x + 0.5) / WID) - 1) * tan(FOV / 2 * M_PI / 180) * ratio; 
	float py = (1 - 2 * ((y + 0.5) / HEI)) * tan(FOV / 2 * M_PI / 180);
	t_vector3 direction = ft_vector3_news(px, py, -1);
	direction = ft_vector3_normalize(&direction);
	return (direction);
}

int		render(t_vars *vars)
{
	static t_vector3 *rot;
	static float cam_y_rot;
	static float cam_x_pos;

	if (!rot)
		rot = ft_vector3_new(1, 0, 1);

	t_data	img;
	img.img = mlx_new_image(vars->mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
	&img.endian);

	t_camera	*camera = new_camera(ft_vector3_new(cam_x_pos, 0, 0), ft_vector3_new(0, 0, 0), FOV);

	t_list		*lights = ft_lst_new(&free_light);
	ft_lst_push(lights, new_light(0.7, ft_vector3_new(20, 0, 0), ft_color_clone(ft_color_from_rgb(255, 255, 255))));
	ft_lst_push(lights, new_light(0.5, ft_vector3_new(20, 20, 0), ft_color_clone(ft_color_from_rgb(255, 255, 255))));
	ft_lst_push(lights, new_light(0, ft_vector3_new(-20, -20, -30), ft_color_clone(ft_color_from_rgb(255, 255, 255))));
	ft_lst_push(lights, new_light(1, ft_vector3_new(0, 0, -35), ft_color_clone(ft_color_from_rgb(255, 255, 255))));

	t_list		*plans = ft_lst_new(&free_plan);
	ft_lst_push(plans, new_plan(ft_vector3_new(0, -800, -40), ft_vector3_clone(ft_vector3_new(0, 0, 1)), ft_color_clone(ft_color_from_rgb(150, 50, 150))));
	ft_lst_push(plans, new_square(10, ft_vector3_new(0, 0, -40), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(255, 0, 0))));
	ft_lst_push(plans, new_square(8, ft_vector3_new(10, 0, -39), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(0, 255, 0))));
	ft_lst_push(plans, new_square(6, ft_vector3_new(-20, 0, -38), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(0, 0, 255))));

	for (size_t i = 0; i < WID; i++)
	{
		for (size_t j = 0; j < HEI; j++)
		{
			t_vector3		ray = compute_ray(camera, i, j);

			t_vector3		pHit;
			t_vector3		nHit;
			float minDist =	INFINITY;
			t_plan			*obj = 0;

			t_iterator		*objectIterator = ft_iterator_new(plans);

			while (ft_iterator_has_next(objectIterator))
			{
				t_plan *plan = ft_iterator_next(objectIterator);
				
				t_vector3 point;
				if (plan_collision(plan, camera->position, &ray, &point))
				{
					float distance = ft_vector3_distancev(&point, camera->position);
					if (distance < minDist)
					{
						pHit = point;
						minDist = distance;
						obj = plan;
					}
				}
			}
			free(objectIterator);

			if (!obj)
				continue;

			t_iterator		*lightIterator = ft_iterator_new(lights);
			t_color			color = *(obj->color);

			float lightAmt = 0;
			while (ft_iterator_has_next(lightIterator))
			{
				t_light *light = ft_iterator_next(lightIterator);

				t_vector3 lightDir = ft_vector3_subv(light->position, &pHit);
				// square of the distance between hitPoint and the light
				float lightDistance2 = ft_vector3_length_squared(&lightDir); 
				lightDir = ft_vector3_normalize(&lightDir);
				float LdotN = fabs(ft_vector3_dotv(&lightDir, obj->rotation));
				float tNearShadow = INFINITY; 
				// is the point in shadow, and is the nearest occluding object closer to the object than the light itself?
				short inShadow = FALSE;

				objectIterator = ft_iterator_new(plans);
				while (ft_iterator_has_next(objectIterator))
				{
					t_vector3 point;
					t_plan *plan = ft_iterator_next(objectIterator);
					if ((inShadow = plan_collision(plan, &pHit, &lightDir, &point)))
					{
						if (obj == plan)
							inShadow = FALSE;
						else
							break;
					}
				}
				free(objectIterator);
				lightAmt += (1 - inShadow) * light->brightness * LdotN;
			}
			lightAmt = fmax(0.2, lightAmt);
			color = ft_color_muld(color, lightAmt);
			set_pixel(&img, i, j, ft_color_to_hexa(color));
		}
	}
	ft_lst_clear(plans);

	mlx_put_image_to_window(vars->mlx, vars->win, img.img, 0, 0);
	// mlx_string_put(vars->mlx, vars->win, WID / 2 - 100 - 4, HEI / 2 - 30 + 3, 0x00CC43BA, "x");
	
	t_vector3 newrot = ft_vector3_rotate_y(*rot, M_PI / (360 / 10));
	free(rot);
	rot = ft_vector3_clone(&newrot);
	// cam_y_rot += 0.1;
	// cam_x_pos += 4;

	free(plans);
	free(lights);
	free(camera);
	return (0);
}

#include <sys/time.h>

int		main(void)
{
	t_vars	vars;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, WID, HEI, "MiniRT - file.rt");

	struct timeval stop, start;
	gettimeofday(&start, NULL);

	render(&vars);
	// mlx_loop_hook(vars.mlx, &render, &vars);
	mlx_key_hook(vars.win, &key_pressed, &vars);

	gettimeofday(&stop, NULL);
	printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
	printf("int           : %lu\nfloat         : %lu\nt_bipredicate : %lu\nt_vector3     : %lu\nt_color       : %lu\nt_square      : %lu\nu_data        : %lu\nt_plan        : %lu\nt_ray         : %lu\n",
		sizeof(int),
		sizeof(float),
		sizeof(t_bipredicate),
		sizeof(t_vector3),
		sizeof(t_color),
		sizeof(t_square),
		sizeof(union u_data),
		sizeof(t_plan),
		sizeof(t_ray));

	mlx_loop(vars.mlx);
}
