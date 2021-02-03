/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/03 13:20:18 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_plan	*new_square(float width, t_vector3 *position, t_vector3 *rotation,
	t_color *color)
{
	t_plan	*plan;

	if (!(plan = new_default_plan(position, rotation, color, &square_collides)))
		return (NULL);
	plan->data.square.width = width;
	return (plan);
}

int			square_collides(void *a1, void *a2)
{
	t_ray		*ray;
	t_plan		*plan;
	t_vector3	v;
	float		d2;

	plan = a1;
	ray = a2;
	v = vec3_subv(ray->phit, *(plan->position));
	d2 = vec3_length_squared(v);
	return (d2 <= plan->data.square.width * plan->data.square.width);
}
