/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 18:46:03 by scros             #+#    #+#             */
/*   Updated: 2021/01/26 16:53:16 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAN_H
# define PLAN_H

# include "vector3.h"
# include "color.h"
# include "types.h"

typedef struct		s_square
{
	int				width;
}					t_square;

typedef struct		s_plan
{
	t_color			*color;
	t_vector3		*position;
	t_vector3		*rotation;
	t_bipredicate	collides;
	union			u_data
	{
		t_square	square;
	}				data;
}					t_plan;

t_plan				*new_plane(t_vector3 *position, t_vector3 *rotation,
	t_color *color, t_bipredicate collides);
short				plan_collision(t_plan *plan, t_vector3 *origin, t_vector3
	*direction, t_vector3 *hit);

#endif
