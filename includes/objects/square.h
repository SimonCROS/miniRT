/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 18:46:03 by scros             #+#    #+#             */
/*   Updated: 2021/01/26 14:35:28 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SQUARE_H
# define SQUARE_H

# include "vector3.h"
# include "color.h"

typedef struct	s_square
{
	t_plan		*plan;
	float		width;
}				t_square;

t_square		*new_square(float width, t_vector3 *position, t_vector3 *rotation,
	t_color *color);
short			square_collision(t_square *square, t_vector3 *origin, t_vector3 *direction, t_vector3 *pHit);

#endif
