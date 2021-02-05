/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/05 12:47:31 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// t_plan	*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3,
// 	t_color *color)
// {
// 	t_plan	*plan;
// 	t_vector3 dir1;
// 	t_vector3 dir2;
// 	t_vector3 rotation;

// 	if (!(plan = new_default_plan(p1, rotation, color, &collides_triangle)))
// 		return (NULL);
// 	plan->data.triangle.width = width;
// 	return (plan);
// }

// int			collides_triangle(void *a1, void *a2)
// {
// 	t_ray		*ray;
// 	t_plan		*plan;
// 	t_vector3	v;
// 	float		d2;

// 	plan = a1;
// 	ray = a2;
// 	v = vec3_subv(ray->phit, *(plan->position));
// 	d2 = vec3_length_squared(v);
// 	return (d2 <= plan->data.triangle.width * plan->data.triangle.width);
// }
