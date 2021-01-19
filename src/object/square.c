/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/01/19 13:03:06 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_square	*new_square(float width, t_vector3 *position, t_vector3 *rotation,
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

short		square_collision(t_square *square, t_vector3 *l0,
	t_vector3 *l, t_vector3 *pHit)
{
	float		t;
	float		d2;
	t_vector3	p;
	t_vector3	v;

	t = 0;
	if (intersect_plane(square->rotation, square->position, l0, l, &t))
	{
		p = ft_vector3_copy(l);
		p = ft_vector3_muld(&p, t);
		p = ft_vector3_addv(&p, l0);
		*pHit = p;
		v = ft_vector3_copy(&p);
		v = ft_vector3_subv(&v, square->position);
		d2 = ft_vector3_length_squared(&v);
		return (d2 <= square->width * square->width);
	}
	return (0);
}
