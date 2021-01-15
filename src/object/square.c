/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/01/15 18:10:12 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_square	*new_square(int width, t_vector3 *position, t_vector3 *rotation,
	t_color *color)
{
	t_square	*square;

	if (!(square = malloc(sizeof(t_square))))
		return (NULL);
	square->width = width;
	square->position = position;
	square->rotation = rotation;
	square->color = color;
	return (square);
}

#include "stdio.h"

short		intersect_plane(const t_vector3 *n, const t_vector3 *p0,
	const t_vector3 *l0, const t_vector3 *l, float *t) 
{ 
	float		denom;
	t_vector3	p0l0;

	denom = ft_vector3_dotv(n, l);
	if (denom > 1e-6)
	{
		ft_vector3_subv(ft_vector3_copy(&p0l0, p0), l0);
		*t = ft_vector3_dotv(&p0l0, n) / denom;
		return (*t >= 0);
	}
	return (0);
} 

short		square_collision(t_square *square, t_vector3 *l0,
	t_vector3 *l, t_vector3 *pHit, t_vector3 *nHit)
{
	float		t;
	float		d2;
	t_vector3	p;
	t_vector3	v;

	t = 0;
	if (intersect_plane(square->rotation, square->position, l0, l, &t))
	{
		ft_vector3_addv(ft_vector3_muld(ft_vector3_copy(&p, l), t), l0);
		ft_vector3_subv(ft_vector3_copy(&v, &p), square->position);
		d2 = ft_vector3_dotv(&v, &v);
		return (d2 <= 850 * 850);
	}
	return (0);
}
