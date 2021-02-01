/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:51:16 by scros             #+#    #+#             */
/*   Updated: 2021/02/01 14:20:01 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			plan_collides(void *vplan, void *vray)
{
	(void)vplan;
	(void)vray;
	return (1);
}

t_plan	*new_plan(t_vector3 *position, t_vector3 *rotation, t_color *color)
{
	t_plan	*plan;

	if (!(plan = new_default_plan(position, rotation, color, &plan_collides)))
		return (NULL);
	return (plan);
}

t_plan	*new_default_plan(t_vector3 *position, t_vector3 *rotation, t_color *color,
	t_bipredicate collides)
{
	t_plan	*plan;

	if (!(plan = malloc(sizeof(t_plan))))
		return (NULL);
	plan->position = position;
	*rotation = ft_vector3_normalize(rotation);
	plan->rotation = rotation;
	plan->collides = collides;
	plan->color = color;
	return (plan);
}

short		intersect_plan(const t_vector3 *n, const t_vector3 *p0,
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
	if (intersect_plan(plan->rotation, plan->position, l0, l, &t))
	{
		p = ft_vector3_copy(l);
		p = ft_vector3_muld(&p, t);
		p = ft_vector3_addv(&p, l0);
		*pHit = p;
		return (plan->collides(plan, pHit));
	}
	return (0);
}
