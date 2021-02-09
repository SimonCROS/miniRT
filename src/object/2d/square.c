/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/09 13:24:42 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			collides_square(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object		*plan;
	t_vector3	c1;
	t_vector3	c2;
	t_vector3	c3;
	t_vector3	edge1;
	t_vector3	edge2;
	t_vector3	edge3;

	plan = arg1;
	ray = arg2;

	edge1 = vec3_subv(plan->data.square.p2, plan->data.square.p1);
	edge2 = vec3_subv(plan->data.square.p3, plan->data.square.p2);
	edge3 = vec3_subv(plan->data.square.p1, plan->data.square.p3);
	c1 = vec3_subv(ray->phit, plan->data.square.p1);
	c2 = vec3_subv(ray->phit, plan->data.square.p2);
	c3 = vec3_subv(ray->phit, plan->data.square.p3);
	if (vec3_dotv(plan->rotation, vec3_crossv(edge1, c1)) > 0 &&
		vec3_dotv(plan->rotation, vec3_crossv(edge2, c2)) > 0 &&
	    vec3_dotv(plan->rotation, vec3_crossv(edge3, c3)) > 0)
			return (1);

	edge1 = vec3_subv(plan->data.square.p3, plan->data.square.p1);
	edge2 = vec3_subv(plan->data.square.p4, plan->data.square.p3);
	edge3 = vec3_subv(plan->data.square.p1, plan->data.square.p4);
	c1 = vec3_subv(ray->phit, plan->data.square.p1);
	c2 = vec3_subv(ray->phit, plan->data.square.p3);
	c3 = vec3_subv(ray->phit, plan->data.square.p4);
	if (vec3_dotv(plan->rotation, vec3_crossv(edge1, c1)) > 0 &&
		vec3_dotv(plan->rotation, vec3_crossv(edge2, c2)) > 0 &&
	    vec3_dotv(plan->rotation, vec3_crossv(edge3, c3)) > 0)
			return (1);
	return (0);
}

t_object	*new_square(float width, t_vector3 position, t_vector3 rotation,
	t_color color)
{
	t_object		*plan;
	float		mid;
	t_vector3	diagonal;

	if (!(plan = new_default_plan(position, rotation, color, &collides_square)))
		return (NULL);
	mid = (width * sqrtf(2)) / 2;
	diagonal = vec3_muld(vec3_normalize(vec3_crossv(rotation, position)), mid);
	diagonal = vec3_rotate_axis(diagonal, rotation, M_PI_4);
	plan->data.square.p1 = vec3_addv(diagonal, position);
	plan->data.square.p3 = vec3_addv(vec3_negate(diagonal), position);
	diagonal = vec3_muld(vec3_normalize(vec3_rotate_axis(diagonal, rotation, M_PI_2)), mid);
	plan->data.square.p2 = vec3_addv(diagonal, position);
	plan->data.square.p4 = vec3_addv(vec3_negate(diagonal), position);
	plan->data.square.width = width;
	printf("%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n",
		plan->data.square.p1.x,
		plan->data.square.p1.y,
		plan->data.square.p1.z,
		plan->data.square.p2.x,
		plan->data.square.p2.y,
		plan->data.square.p2.z,
		plan->data.square.p3.x,
		plan->data.square.p3.y,
		plan->data.square.p3.z,
		plan->data.square.p4.x,
		plan->data.square.p4.y,
		plan->data.square.p4.z);
	return (plan);
}
