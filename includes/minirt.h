/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 12:31:12 by scros             #+#    #+#             */
/*   Updated: 2021/02/16 13:41:16 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// # define WID 2560
// # define HEI 1440

# define WID 960
# define HEI 540

# define RENDER_WID 100
# define RENDER_HEI 40

// # define WID (960 / 4)
// # define HEI (540 / 4)

// # define WID 100
// # define HEI 50

# define FOV 60

# include <math.h>
# include "libft.h"
# include "mlx.h"

typedef struct		s_data {
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}					t_data;

typedef struct	s_ray {
	t_vector3		direction;
	t_vector3		phit;
	t_vector3		nhit;
	t_vector3		*origin;
	t_color			color;
	float			length;
	float			light;
}					t_ray;

typedef struct		s_vars {
	void			*mlx;
	void			*win;
}					t_vars;

typedef struct		s_scene {
	t_list			*cameras;
	t_list			*lights;
	t_list			*objects;
	int				camera_index;
}					t_scene;

# include "object.h"

void				set_pixel(t_data *data, int x, int y, int color);

#endif
