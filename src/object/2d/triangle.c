/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/05 13:30:16 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			collides_triangle(void *a1, void *a2)
{
	t_ray		*ray;
	t_plan		*plan;
	t_vector3	v;
	float		d2;

	plan = a1;
	ray = a2;
	v = vec3_subv(ray->phit, plan->position);
	d2 = vec3_length_squared(v);
	return (d2 <= 50);
}

t_plan	*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3, t_color color)
{
	t_plan	*plan;
	t_vector3 center;
	t_vector3 dir1;
	t_vector3 dir2;
	t_vector3 rotation;

	center = vec3_divd(vec3_addv(p1, vec3_addv(p2, p3)), 3);
	dir1 = vec3_subv(p2, p1);
	dir2 = vec3_subv(p3, p1);
	rotation = vec3_crossv(dir1, dir2);
	if (!(plan = new_default_plan(center, rotation, color, &collides_triangle)))
		return (NULL);
	plan->data.triangle.point = p1;
	plan->data.triangle.dir1 = dir1;
	plan->data.triangle.dir2 = dir2;
	return (plan);
}
