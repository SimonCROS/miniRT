/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 13:03:09 by scros             #+#    #+#             */
/*   Updated: 2021/01/16 17:18:46 by scros            ###   ########lyon.fr   */
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

	t_light		*light = new_light(0.6, ft_vector3_new(1000, WID / 2 - 200, HEI / 2), ft_color_from_rgb(255, 0, 0));

	t_list		*list = ft_lst_new(&free_square);
	ft_lst_push(list, new_square(1, ft_vector3_new(1000, WID / 2, HEI / 2), ft_vector3_new(0.2, 0.2, 0.2), ft_color_from_rgb(255, 0, 0)));
	ft_lst_push(list, new_square(1, ft_vector3_new(1000, WID / 2 - 100, HEI / 2 - 50), ft_vector3_new(0.2, 0.2, 0.2), ft_color_from_rgb(0, 255, 0)));
	ft_lst_push(list, new_square(1, ft_vector3_new(1000, WID / 2 + 100, HEI / 2 + 150), ft_vector3_new(0.2, 0.2, -0.2), ft_color_from_rgb(0, 0, 255)));

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
			float minDist =	-1;
			t_square *obj = 0;

			t_iterator		*iterator = ft_iterator_new(list);

			while (ft_iterator_has_next(iterator))
			{
				ft_putendl("aaa");
				t_square *square = ft_iterator_next(iterator);
				ft_putendl("bbb");
				if (square_collision(square, &origin, &ray, &pHit, &nHit))
				{
					obj = square;
				}
			}
			if (obj)
			{
				//t_vector3	dir = ft_vector3_subv(light->position, pHit);

				set_pixel(&img, i, j, ft_color_to_hexa(obj->color));
			}
		}
	}
	ft_lst_clear(list);

	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	mlx_key_hook(vars.win, &key_pressed, &vars);
	mlx_loop(vars.mlx);
}
