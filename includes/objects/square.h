/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 18:46:03 by scros             #+#    #+#             */
/*   Updated: 2021/01/15 13:34:28 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SQUARE_H
# define SQUARE_H

# include "vector3.h"
# include "color.h"

typedef struct	s_square
{
	int			width;
	t_vector3	*position;
	t_vector3	*rotation;
	t_color		*color;
}				t_square;

t_square		*new_square(int width, t_vector3 *position, t_vector3 *rotation,
	t_color *color);
short			square_collision(t_square *square, t_vector3 *vector, t_vector3 *start, t_vector3 *pHit, t_vector3 *nHit);

#endif
