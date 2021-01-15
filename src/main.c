/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/01/15 18:01:14 by scros            ###   ########lyon.fr   */
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

int		main(void)
{
	t_vars	vars;
	t_data	img;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, WID, HEI, "MiniRT - file.rt");
	img.img = mlx_new_image(vars.mlx, WID, HEI);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
	&img.endian);

	t_vector3 origin;
	ft_vector3_init(&origin, -FOV, WID / 2, HEI / 2);
	t_square *square = new_square(1, ft_vector3_new(1000, WID / 2, HEI / 2), 
		ft_vector3_new(1, 0, 1), ft_color_from_rgb(255, 0, 0));
	for (size_t i = 0; i < WID; i++)
	{
		for (size_t j = 0; j < HEI; j++)
		{
			t_vector3 ray;
			ft_vector3_copy(&ray, ft_vector3_new(0, i, j));
			ft_vector3_subv(&ray, &origin);
			ft_vector3_normalize(&ray);

			t_vector3		pHit;
			t_vector3		nHit;
			float minDist =	-1;
			t_square *obj = 0;
			if (square_collision(square, &origin, &ray, &pHit, &nHit))
			{
				obj = square;
			}
			if (obj)
			{
				set_pixel(&img, i, j, ft_color_to_hexa(obj->color));
			}
			else
				set_pixel(&img, i, j, 0x00ffffff);
		}
	}

	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	mlx_key_hook(vars.win, &key_pressed, &vars);
	mlx_loop(vars.mlx);
}
