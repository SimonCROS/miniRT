/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/01/26 15:57:59 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_plan	*new_plan(t_vector3 *position, t_vector3 *rotation, t_color *color)
{
	t_plan	*plan;

	if (!(plan = malloc(sizeof(t_plan))))
		return (NULL);
	plan->position = position;
	plan->rotation = rotation;
	plan->color = color;
	return (plan);
}

#include "stdio.h"

short		intersect_plane(const t_vector3 *n, const t_vector3 *p0,
	const t_vector3 *l0, const t_vector3 *l, float *t) 
{ 
	float		dot;
	t_vector3	p0l0;

	dot = ft_vector3_dotv(n, l);
	if (dot != 0)
	{
		p0l0 = ft_vector3_subv(p0, l0);
		*t = ft_vector3_dotv(&p0l0, n) / dot;
		return (*t >= 0);
	}
	return (0);
}

short		plan_collision(t_plan *plan, t_vector3 *l0,
	t_vector3 *l, t_vector3 *pHit)
{
	float		t;
	t_vector3	p;

	t = 0;
	if (intersect_plane(plan->rotation, plan->position, l0, l, &t))
	{
		p = ft_vector3_copy(l);
		p = ft_vector3_muld(&p, t);
		p = ft_vector3_addv(&p, l0);
		*pHit = p;
		return (plan->collides(plan, pHit));
	}
	return (0);
}

int			plan_collides(t_plan *plan, t_vector3 hit)
{
	return (1);
}

int			square_collides(t_plan *plan, t_vector3 *hit)
{
	t_vector3	v;
	float		d2;

	v = ft_vector3_subv(hit, plan->position);
	d2 = ft_vector3_length_squared(&v);
	return (d2 <= plan->width * plan->width);
}
