/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/01/29 16:17:36 by scros            ###   ########lyon.fr   */
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

void debug(void *plan)
{
	printf("%#.8X\n", ft_color_to_hexa(*((t_plan*)plan)->color));
}

int		render(t_vars *vars)
{
	static t_vector3 *rot;
	static float cam_y_rot;
	static float cam_x_pos;

	if (!rot)
		rot = ft_vector3_new(1, 0, 0.4);

	t_data	img;
	img.img = mlx_new_image(vars->mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
	&img.endian);

	t_camera	*camera = new_camera(ft_vector3_new(cam_x_pos, 0, 0), ft_vector3_new(0, 0, 0), FOV);

	t_list		*lights = ft_lst_new(&free_light);
	ft_lst_push(lights, new_light(0.6, ft_vector3_new(-20, 0, -20), ft_color_clone(ft_color_from_rgb(255, 255, 255))));

	t_list		*plans = ft_lst_new(&free_plan);
	ft_lst_push(plans, new_plan(ft_vector3_new(0, 0, -50), ft_vector3_clone(ft_vector3_new(0, 1, 1)), ft_color_clone(ft_color_from_rgb(150, 50, 150))));
	ft_lst_push(plans, new_square(10, ft_vector3_new(0, 0, -40), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(255, 0, 0))));
	ft_lst_push(plans, new_square(8, ft_vector3_new(0, 0, -39), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(0, 255, 0))));
	ft_lst_push(plans, new_square(6, ft_vector3_new(0, 0, -38), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(0, 0, 255))));

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
			t_color			color = ft_color_from_hex(0x00000000);

			while (ft_iterator_has_next(lightIterator))
			{
				t_light *light = ft_iterator_next(lightIterator);
			
				short hit = 0;
				t_vector3 dir = ft_vector3_subv(light->position, &pHit);
				dir = ft_vector3_normalize(&dir);

				color = *(obj->color);

				objectIterator = ft_iterator_new(plans);
				while (ft_iterator_has_next(objectIterator))
				{
					t_vector3 point;
					t_plan *plan = ft_iterator_next(objectIterator);
					if ((hit = plan_collision(plan, &pHit, &dir, &point)))
					{
						if (obj == plan)
							hit = 0;
						else
							break;
					}
				}

				if (hit)
					color = ft_color_divd(color, 2);
				else {
					float ratio = fmax(0, ft_vector3_dotv(&dir, obj->rotation));
					t_color lc = *(light->color);
					lc = ft_color_muld(lc, ratio);
					color = ft_color_mul(color, lc.r / (float) 255, lc.g / (float) 255, lc.b / (float) 255);
				}
				free(objectIterator);
			}
			set_pixel(&img, i, j, ft_color_to_hexa(color));
		}
	}
	ft_lst_clear(plans);

	mlx_put_image_to_window(vars->mlx, vars->win, img.img, 0, 0);
	// mlx_string_put(vars->mlx, vars->win, WID / 2 - 100 - 4, HEI / 2 - 30 + 3, 0x00CC43BA, "x");
	
	// t_vector3 newrot = ft_vector3_rotate_z(*rot, M_PI / (360 / 10));
	// free(rot);
	// rot = ft_vector3_clone(&newrot);
	// cam_y_rot -= 0.1;
	cam_x_pos += 4;

	free(plans);
	free(lights);
	free(camera);
	return (0);
}

int		main(void)
{
	t_vars	vars;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, WID, HEI, "MiniRT - file.rt");

	render(&vars);
	// mlx_loop_hook(vars.mlx, &render, &vars);
	mlx_key_hook(vars.win, &key_pressed, &vars);

	mlx_loop(vars.mlx);
}
