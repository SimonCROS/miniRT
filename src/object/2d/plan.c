/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/01/26 16:53:51 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_plan	*new_plan(float width, t_vector3 *position, t_vector3 *rotation,
	t_color *color)
{
	t_plan	*plan;

	if (!(plan = new_plane(position, rotation, color, &plan_collides)))
		return (NULL);
	return (plan);
}

int			plan_collides(t_plan *plan, t_vector3 hit)
{
	return (1);
}
