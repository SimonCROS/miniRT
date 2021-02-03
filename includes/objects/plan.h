/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 18:46:03 by scros             #+#    #+#             */
/*   Updated: 2021/02/03 13:20:45 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAN_H
# define PLAN_H

# include "minirt.h"
# include "vector3.h"
# include "color.h"
# include "types.h"

typedef struct		s_square
{
	float			width;
}					t_square;

union				u_data
{
	t_square		square;
};

typedef struct		s_plan
{
	t_bipredicate		collides;
	t_vector3		*position;
	t_vector3		*rotation;
	t_color			*color;
	union u_data	data;
}					t_plan;

t_plan				*new_plan(t_vector3 *position, t_vector3 *rotation, t_color *color);
t_plan				*new_default_plan(t_vector3 *position, t_vector3 *rotation, t_color *color, t_bipredicate collides);
short				plan_collision(t_plan *plan, t_ray *ray);
int					plan_collides(void *plan, void *ray);


#endif
