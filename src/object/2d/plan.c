/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:51:16 by scros             #+#    #+#             */
/*   Updated: 2021/02/09 13:34:45 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			collides_plan(void *plan, void *ray)
{
	(void)plan;
	(void)ray;
	return (1);
}

t_object	*new_plan(t_vector3 position, t_vector3 rotation, t_color color)
{
	t_object	*plan;

	if (!(plan = new_default_plan(position, rotation, color, &collides_plan)))
		return (NULL);
	return (plan);
}

t_object	*new_default_plan(t_vector3 position, t_vector3 rotation, t_color color, t_bipredicate collides)
{
	t_object	*plan;

	if (!(plan = new_default_object(position, rotation, color, collides)))
		return (NULL);
	plan->is_plan = 1;
	return (plan);
}

t_object	*new_default_object(t_vector3 position, t_vector3 rotation, t_color color, t_bipredicate collides)
{
	t_object	*object;

	if (!(object = malloc(sizeof(t_object))))
		return (NULL);
	object->position = position;
	object->rotation = vec3_normalize(rotation);
	object->collides = collides;
	object->color = color;
	object->is_plan = 0;
	return (object);
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

int			collision(t_object *object, t_ray *ray)
{
	float		t;
	t_vector3	p;

	if (!object->is_plan)
		return (object->collides(object, ray));
	else if (intersect_plan(&object->rotation, &object->position, ray->origin, ray->direction, &(ray->length)))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, *(ray->origin));
		ray->phit = p;
		ray->nhit = object->rotation;
		return (object->collides(object, ray));
	}
	return (0);
}
