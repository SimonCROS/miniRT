/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:51:16 by scros             #+#    #+#             */
/*   Updated: 2021/02/17 14:34:45 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_object	*new_plan(t_vector3 pos, t_vector3 rot, t_color col)
{
	t_object	*plan;

	return (new_default_plan(pos, rot, col, NULL));
}

t_object	*new_default_plan(t_vector3 pos, t_vector3 rot, t_color col,
	t_bipredicate collides)
{
	t_object	*plan;

	plan = new_default_object(pos, rot, col, collides);
	if (!plan)
		return (NULL);
	plan->is_plan = 1;
	return (plan);
}

t_object	*new_default_object(t_vector3 pos, t_vector3 rot, t_color col,
	t_bipredicate collides)
{
	t_object	*object;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	object->position = pos;
	object->rotation = vec3_normalize(rot);
	object->collides = collides;
	object->color = col;
	object->is_plan = 0;
	return (object);
}

int	intersect_plan(t_vector3 pos, t_vector3 rot, t_ray *ray)
{
	float		dot;
	t_vector3	p0l0;

	dot = vec3_dotv(rot, ray->direction);
	if (dot != 0)
	{
		p0l0 = vec3_subv(pos, ray->origin);
		ray->length = vec3_dotv(p0l0, rot) / dot;
		return (ray->length >= 0);
	}
	return (0);
}

int	collision(t_object *object, t_ray *ray)
{
	t_vector3	p;

	if (!object->is_plan)
		return (object->collides(object, ray));
	else if (intersect_plan(object->position, object->rotation, ray))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, ray->origin);
		ray->phit = p;
		ray->nhit = object->rotation;
		if (vec3_dotv(object->rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		if (!object->collides)
			return (1);
		return (object->collides(object, ray));
	}
	return (0);
}
