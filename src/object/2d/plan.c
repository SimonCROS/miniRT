/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:51:16 by scros             #+#    #+#             */
/*   Updated: 2021/02/05 13:24:45 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			collides_plan(void *plan, void *ray)
{
	(void)plan;
	(void)ray;
	return (1);
}

t_plan	*new_plan(t_vector3 position, t_vector3 rotation, t_color color)
{
	t_plan	*plan;

	if (!(plan = new_default_plan(position, rotation, color, &collides_plan)))
		return (NULL);
	return (plan);
}

t_plan	*new_default_plan(t_vector3 position, t_vector3 rotation, t_color color, t_bipredicate collides)
{
	t_plan	*plan;

	if (!(plan = malloc(sizeof(t_plan))))
		return (NULL);
	plan->position = position;
	plan->rotation = vec3_normalize(rotation);
	plan->collides = collides;
	plan->color = color;
	return (plan);
}

short		intersect_plan(const t_vector3 *n, const t_vector3 *p0,
	const t_vector3 *l0, t_vector3 l, float *t) 
{
	float		dot;
	t_vector3	p0l0;

	dot = vec3_dotv(*n, l);
	if (dot != 0)
	{
		p0l0 = vec3_subv(*p0, *l0);
		*t = vec3_dotv(p0l0, *n) / dot;
		return (*t >= 0);
	}
	return (0);
}

int			plan_collision(t_plan *plan, t_ray *ray)
{
	float		t;
	t_vector3	p;

	if (intersect_plan(&plan->rotation, &plan->position, ray->origin, ray->direction, &(ray->length)))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, *(ray->origin));
		ray->phit = p;
		ray->nhit = plan->rotation;
		return (plan->collides(plan, ray));
	}
	return (0);
}
