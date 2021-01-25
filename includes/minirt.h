/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 12:31:12 by scros             #+#    #+#             */
/*   Updated: 2021/01/25 13:52:53 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#define WID 960
#define HEI 540
// #define WID 5
// #define HEI 5

#define FOV 0

# include <math.h>
# include "libft.h"
# include "mlx.h"
# include "object.h"

typedef struct	s_data {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_data;

typedef struct	s_vars {
	void		*mlx;
	void		*win;
}				t_vars;

void			set_pixel(t_data *data, int x, int y, int color);

#endif
