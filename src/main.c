/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/01/17 16:29:52 by scros            ###   ########lyon.fr   */
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

#define FOV 100

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

int		main(void)
{
	t_vars	vars;
	t_data	img;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, WID, HEI, "MiniRT - file.rt");
	img.img = mlx_new_image(vars.mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
	&img.endian);

	t_vector3 origin = ft_vector3_news(-FOV, WID / 2, HEI / 2);

	t_list		*lights = ft_lst_new(&free_light);
	ft_lst_push(lights, new_light(0.6, ft_vector3_new(0, WID / 2 + 20, HEI / 2 - 30), ft_color_clone(ft_color_from_rgb(255, 255, 255))));

	t_list		*squares = ft_lst_new(&free_square);
	ft_lst_push(squares, new_square(50, ft_vector3_new(0, WID / 2, HEI / 2), ft_vector3_new(0.2, 0.4, 0.2), ft_color_clone(ft_color_from_rgb(255, 0, 0))));
	ft_lst_push(squares, new_square(60, ft_vector3_new(20, WID / 2, HEI / 2), ft_vector3_new(0.2, 0.2, 0.2), ft_color_clone(ft_color_from_rgb(0, 255, 0))));
	ft_lst_push(squares, new_square(15, ft_vector3_new(0, WID / 2 + 100, HEI / 2 + 150), ft_vector3_new(0.2, 0.2, -0.2), ft_color_clone(ft_color_from_rgb(0, 0, 255))));

	for (size_t i = 0; i < WID; i++)
	{
		for (size_t j = 0; j < HEI; j++)
		{
			t_vector3 ray;
			ray = ft_vector3_news(0, i, j);
			ray = ft_vector3_subv(&ray, &origin);
			ray = ft_vector3_normalize(&ray);

			t_vector3		pHit;
			t_vector3		nHit;
			float minDist =	INFINITY;
			t_square *obj = 0;

			t_iterator		*objectIterator = ft_iterator_new(squares);

			while (ft_iterator_has_next(objectIterator))
			{
				t_square *square = ft_iterator_next(objectIterator);
				
				t_vector3 point;
				if (square_collision(square, &origin, &ray, &point))
				{
					float distance = ft_vector3_distancev(&point, &origin);
					if (minDist < 0 || distance < minDist)
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

			t_iterator		*lightIterator = ft_iterator_new(squares);

			while (ft_iterator_has_next(lightIterator))
			{
				t_square *light = ft_iterator_next(lightIterator);
			
				short hit = 0;
				t_vector3 dir = ft_vector3_subv(light->position, &pHit);
				dir = ft_vector3_normalize(&dir);

				objectIterator = ft_iterator_new(squares);

				while (ft_iterator_has_next(objectIterator))
				{
					t_vector3 point;
					t_square *square = ft_iterator_next(objectIterator);
					if ((hit = square_collision(square, &pHit, &dir, &point)))
					{
						if (obj == square && ft_vector3_distancev(&pHit, &point) != 0)
							hit = 0;
						else
							break;
					}
				}
				if (!hit)
					set_pixel(&img, i, j, ft_color_to_hexa(*(obj->color)));
				else
					set_pixel(&img, i, j, 0x00101010);
				free(objectIterator);
			}
		}
	}
	ft_lst_clear(squares);

	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	// mlx_string_put(vars.mlx, vars.win, WID / 2 - 50, HEI / 2 + 80, 0x00CC43BA, "x");
	mlx_key_hook(vars.win, &key_pressed, &vars);
	mlx_loop(vars.mlx);
}
