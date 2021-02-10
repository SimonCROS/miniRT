/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 12:31:12 by scros             #+#    #+#             */
/*   Updated: 2021/02/10 10:45:58 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# define WID 960
# define HEI 540

// # define WID (960 / 4)
// # define HEI (540 / 4)

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

# include "object.h"

void				set_pixel(t_data *data, int x, int y, int color);

#endif
