/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 18:46:03 by scros             #+#    #+#             */
/*   Updated: 2021/02/10 14:41:05 by scros            ###   ########lyon.fr   */
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

typedef struct		s_sphere
{
	float			radius;
}					t_sphere;

typedef struct		s_cylinder
{
	t_vector3		position2;
	float			radius;
	float			height;
}					t_cylinder;

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
	t_sphere		sphere;
	t_circle		circle;
	t_cylinder		cylinder;
	t_triangle		triangle;
}					t_type;

typedef struct		s_object
{
	t_bipredicate	collides;
	t_vector3		position;
	t_vector3		rotation;
	int				is_plan;
	t_color			color;
	t_type			data;
}					t_object;

t_object			*new_sphere(float diametre, t_vector3 position, t_color color);
t_object			*new_plan(t_vector3 position, t_vector3 rotation, t_color color);
t_object			*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3, t_color color);
t_object			*new_square(float width, t_vector3 position, t_vector3 rotation, t_color color);
t_object			*new_circle(float diametre, t_vector3 position, t_vector3 rotation, t_color color);
t_object			*new_default_plan(t_vector3 position, t_vector3 rotation, t_color color, t_bipredicate collides);
t_object			*new_default_object(t_vector3 position, t_vector3 rotation, t_color color, t_bipredicate collides);
t_object			*new_cylinder(float diametre, float height, t_vector3 position, t_vector3 rotation, t_color color);
int					intersect_plan(t_vector3 position, t_vector3 rotation, t_ray *ray);
int					collision(t_object *object, t_ray *ray);

#endif
