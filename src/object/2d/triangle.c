/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/09 13:34:35 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	collides_triangle(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*plan;
	t_vector3	c1;
	t_vector3	c2;
	t_vector3	c3;
	t_vector3	edge1;
	t_vector3	edge2;
	t_vector3	edge3;

	plan = arg1;
	ray = arg2;
	edge1 = vec3_subv(plan->data.triangle.p2, plan->data.triangle.p1);
	edge2 = vec3_subv(plan->data.triangle.p3, plan->data.triangle.p2);
	edge3 = vec3_subv(plan->data.triangle.p1, plan->data.triangle.p3);
	c1 = vec3_subv(ray->phit, plan->data.triangle.p1);
	c2 = vec3_subv(ray->phit, plan->data.triangle.p2);
	c3 = vec3_subv(ray->phit, plan->data.triangle.p3);
	if (vec3_dotv(plan->rotation, vec3_crossv(edge1, c1)) > 0
		&& vec3_dotv(plan->rotation, vec3_crossv(edge2, c2)) > 0
		&& vec3_dotv(plan->rotation, vec3_crossv(edge3, c3)) > 0)
		return (1);
	return (0);
}

t_object	*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3, t_color col)
{
	t_object	*plan;
	t_vector3	cen;
	t_vector3	d1;
	t_vector3	d2;

	cen = vec3_divd(vec3_addv(p1, vec3_addv(p2, p3)), 3);
	d1 = vec3_subv(p2, p1);
	d2 = vec3_subv(p3, p2);
	plan = new_default_plan(cen, vec3_crossv(d1, d2), col, &collides_triangle);
	if (!plan)
		return (NULL);
	plan->data.triangle.p1 = p1;
	plan->data.triangle.p2 = p2;
	plan->data.triangle.p3 = p3;
	return (plan);
}
