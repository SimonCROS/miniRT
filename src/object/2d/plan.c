/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:51:16 by scros             #+#    #+#             */
/*   Updated: 2021/02/02 12:56:14 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			plan_collides(t_ray *ray)
{
	(void)ray;
	return (1);
}

t_plan	*new_plan(t_vector3 *position, t_vector3 *rotation, t_color *color)
{
	t_plan	*plan;

	if (!(plan = new_default_plan(position, rotation, color, &plan_collides)))
		return (NULL);
	return (plan);
}

t_plan	*t_predicate(t_vector3 *position, t_vector3 *rotation, t_color *color,
	t_bipredicate collides)
{
	t_plan	*plan;

	if (!(plan = malloc(sizeof(t_plan))))
		return (NULL);
	plan->position = position;
	*rotation = vec3_normalize(*rotation);
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

	dot = vec3_dotv(*n, *l);
	if (dot != 0)
	{
		p0l0 = vec3_subv(*p0, *l0);
		*t = vec3_dotv(p0l0, *n) / dot;
		return (*t >= 0);
	}
	return (0);
}

short		plan_collision(t_ray *ray)
{
	float		t;
	t_vector3	p;

	t = 0;
	if (intersect_plan(ray->plan->rotation, ray->plan->position, l0, l, &t))
	{
		p = vec3_muld(l, t);
		p = vec3_addv(p, l0);
		ray->phit = p;
		return (ray->plan->collides(ray));
	}
	return (0);
}
