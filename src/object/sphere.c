/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 14:12:21 by scros             #+#    #+#             */
/*   Updated: 2021/01/17 14:27:20 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_sphere		*new_sphere(float radius, t_vector3 *position, t_color *color){
	t_sphere	*sphere;

	if (!(sphere = malloc(sizeof(t_sphere))))
		return (NULL);
	sphere->radius = radius;
	sphere->position = position;
	sphere->color = color;
	return (sphere);
}

short	sphere_collision(t_sphere *sphere, t_vector3 *origin, t_vector3 *dir, float *t0, float *t1)
{
	t_vector3	l = ft_vector3_subv(sphere->position, origin);
	float		tca = ft_vector3_dotv(&l, dir);
	float		radius_sq = sphere->radius * sphere->radius;

	if (tca < 0)
		return (0);
	float d2 = ft_vector3_length_squared(&l) - tca * tca;
	if (d2 > radius_sq)
		return (0);
	float thc = sqrt(radius_sq - d2);
	*t0 = tca - thc;
	*t1 = tca + thc;
	return (1); 
}
