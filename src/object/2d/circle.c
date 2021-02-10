/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/10 12:48:28 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			collides_circle(void *a1, void *a2)
{
	t_ray		*ray;
	t_object		*plan;
	t_vector3	v;
	float		d2;

	plan = a1;
	ray = a2;
	v = vec3_subv(ray->phit, plan->position);
	d2 = vec3_length_squared(v);
	return (d2 <= plan->data.circle.radius * plan->data.circle.radius);
}

t_object	*new_circle(float diametre, t_vector3 position, t_vector3 rotation,
	t_color color)
{
	t_object	*plan;

	if (!(plan = new_default_plan(position, rotation, color, &collides_circle)))
		return (NULL);
	plan->data.circle.radius = diametre / 2;
	return (plan);
}
