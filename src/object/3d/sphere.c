/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 14:12:21 by scros             #+#    #+#             */
/*   Updated: 2021/02/10 14:37:30 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int			collides_sphere(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*object;
	t_vector3	to_center;
	float		t0, t1;

	object = arg1;
	ray = arg2;

	to_center = vec3_subv(object->position, *(ray->origin));
	float tca = vec3_dotv(to_center, ray->direction); 
	if (tca < 0) return 0;
	float d2 = vec3_dotv(to_center, to_center) - tca * tca;
	float radius2 = object->data.sphere.radius * object->data.sphere.radius;
	if (d2 > radius2) return 0;
	float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;
	if (t0 > t1)
	{
		t0 = t0 + t1;
		t1 = t0 - t1;
		t0 = t0 - t1;
	}
	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) return 0;
	}
	ray->length = t0;
	ray->phit = vec3_muld(ray->direction, ray->length);
	ray->nhit = vec3_normalize(vec3_subv(ray->phit, object->position));
	return 1;
}

t_object		*new_sphere(float diametre, t_vector3 position, t_color color)
{
	t_object	*object;

	if (!(object = new_default_object(position, vec3_new(0, 1, 0), color, &collides_sphere)))
		return (NULL);
	object->data.sphere.radius = diametre / 2;
	return (object);
}
