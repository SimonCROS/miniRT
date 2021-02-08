/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 18:46:03 by scros             #+#    #+#             */
/*   Updated: 2021/02/08 12:35:54 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAN_H
# define PLAN_H

# include "minirt.h"
# include "vector3.h"
# include "color.h"
# include "types.h"

typedef struct		s_circle
{
	float			radius;
}					t_circle;

typedef struct		s_square
{
	float			width;
	t_vector3		p1;
	t_vector3		p2;
	t_vector3		p3;
	t_vector3		p4;
}					t_square;

typedef struct		s_triangle
{
	t_vector3		p1;
	t_vector3		p2;
	t_vector3		p3;
}					t_triangle;

typedef union		u_type
{
	t_square		square;
	t_circle		circle;
	t_triangle		triangle;
}					t_type;

typedef struct		s_plan
{
	t_bipredicate	collides;
	t_vector3		position;
	t_vector3		rotation;
	t_color			color;
	t_type			data;
}					t_plan;

t_plan				*new_plan(t_vector3 position, t_vector3 rotation, t_color color);
t_plan				*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3, t_color color);
t_plan				*new_square(float width, t_vector3 position, t_vector3 rotation, t_color color);
t_plan				*new_circle(float radius, t_vector3 position, t_vector3 rotation, t_color color);
t_plan				*new_default_plan(t_vector3 position, t_vector3 rotation, t_color color, t_bipredicate collides);
int					plan_collision(t_plan *plan, t_ray *ray);


#endif
