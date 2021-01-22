/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/01/20 14:03:05 by scros            ###   ########lyon.fr   */
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

#define WID 960
#define HEI 540

// #define WID 5
// #define HEI 5

#define FOV 831.384

void	free_square(void *p)
{
	t_square *square;
	
	square = (t_square*)p;
	free(square->position);
	free(square->rotation);
	free(square->color);
	free(square);
}

void	free_light(void *p)
{
	t_light *light;
	
	light = (t_light*)p;
	free(light->position);
	free(light->color);
	free(light);
}

void	free_sphere(void *p)
{
	t_sphere *sphere;
	
	sphere = (t_sphere*)p;
	free(sphere->position);
	free(sphere->color);
	free(sphere);
}

t_vector3	compute_ray(t_camera *camera, int i, int j)
{
	t_vector3	ray;
	t_vector3	axis;
	t_vector3	shift;
	t_vector3	direction;
	float		angle;

	direction = ft_vector3_news(1, 0, 0);
	angle = ft_vector3_angle(*camera->direction, direction);
	axis = ft_vector3_crossv(camera->direction, &direction);
	shift = ft_vector3_mulv(camera->position, camera->direction);
	ray = ft_vector3_news(FOV, i, j);
	ray = ft_vector3_rotate_axis(ray, axis, angle);
	ray = ft_vector3_subv(&ray, camera->position);
	ray = ft_vector3_normalize(&ray);	
	return (ray);
}

int		render(t_vars *vars)
{
	static t_vector3 *rot;

	if (!rot)
		rot = ft_vector3_new(1, 1, 0);

	t_data	img;
	img.img = mlx_new_image(vars->mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
	&img.endian);

	t_camera	*camera = new_camera(ft_vector3_new(-800, 0, 0), ft_vector3_new(1, 0, 0));

	t_list		*lights = ft_lst_new(&free_light);
	ft_lst_push(lights, new_light(0.6, ft_vector3_new(0, WID / 2 - 100, HEI / 2 - 30), ft_color_clone(ft_color_from_rgb(255, 255, 255))));

	t_list		*squares = ft_lst_new(&free_square);
	ft_lst_push(squares, new_square(50, ft_vector3_new(0, WID / 2, HEI / 2), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(255, 0, 0))));
	ft_lst_push(squares, new_square(70, ft_vector3_new(-20, WID / 2 + 50, HEI / 2), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(0, 255, 0))));
	ft_lst_push(squares, new_square(90, ft_vector3_new(-40, WID / 2 + 100, HEI / 2), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(0, 0, 255))));
	ft_lst_push(squares, new_square(90, ft_vector3_new(-40, WID / 2 + 300, HEI / 2), ft_vector3_clone(rot), ft_color_clone(ft_color_from_rgb(0, 0, 255))));

	for (size_t i = 0; i < WID; i++)
	{
		for (size_t j = 0; j < HEI; j++)
		{
			t_vector3		ray = compute_ray(camera, i, j);

			t_vector3		pHit;
			t_vector3		nHit;
			float minDist =	INFINITY;
			t_square *obj = 0;

			t_iterator		*objectIterator = ft_iterator_new(squares);

			while (ft_iterator_has_next(objectIterator))
			{
				t_square *square = ft_iterator_next(objectIterator);
				
				t_vector3 point;
				if (square_collision(square, camera->position, &ray, &point))
				{
					float distance = ft_vector3_distancev(&point, camera->position);
					if (distance < minDist)
					{
						pHit = point;
						minDist = distance;
						obj = square;
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
				t_square *light = ft_iterator_next(lightIterator);
			
				short hit = 0;
				t_vector3 dir = ft_vector3_subv(light->position, &pHit);
				dir = ft_vector3_normalize(&dir);

				color = *(obj->color);

				objectIterator = ft_iterator_new(squares);
				while (ft_iterator_has_next(objectIterator))
				{
					t_vector3 point;
					t_square *square = ft_iterator_next(objectIterator);
					if ((hit = square_collision(square, &pHit, &dir, &point)))
					{
						if (obj == square)
							hit = 0;
						else
							break;
					}
				}

				if (hit)
					color = ft_color_divd(color, 2);

				free(objectIterator);
			}
			set_pixel(&img, i, j, ft_color_to_hexa(color));
		}
	}
	ft_lst_clear(squares);

	mlx_put_image_to_window(vars->mlx, vars->win, img.img, 0, 0);
	mlx_string_put(vars->mlx, vars->win, WID / 2 - 100 - 4, HEI / 2 - 30 + 3, 0x00CC43BA, "x");
	
	t_vector3 newrot = ft_vector3_rotate_z(*rot, M_PI / (360 / 10));
	free(rot);
	rot = ft_vector3_clone(&newrot);

	free(squares);
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
