/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/01/29 15:50:53 by scros            ###   ########lyon.fr   */
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

int			square_collides(void *vplan, void *vhit)
{
	t_plan		*plan;
	t_vector3	*hit;
	t_vector3	v;
	float		d2;

	plan = (t_plan*)vplan;
	hit = (t_vector3*)vhit;
	v = ft_vector3_subv(hit, plan->position);
	d2 = ft_vector3_length_squared(&v);
	return (d2 <= plan->data.square.width * plan->data.square.width);
}
