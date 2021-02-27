/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/10 14:18:57 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	rot_deserialize(const char *str, t_vector3 *vector)
{
	t_vector3	rot;

	if (!vec3_deserialize(str, &rot))
		return (0);
	if (rot.x < -1 || rot.x > 1 ||
		rot.y < -1 || rot.y > 1 ||
		rot.z < -1 || rot.z > 1)
		return (0);
	*vector = rot;
	return (1);
}

t_object	*parse_square(t_list *data)
{
	t_vector3	pos;
	t_vector3	rot;
	float		width;
	t_color		color;
	int			e;

	if (data->size != 5)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && rot_deserialize((char *)lst_get(data, 2), &rot);
	e = e && is_float((char *)lst_get(data, 3));
	e = e && color_deserialize((char *)lst_get(data, 4), &color);
	if (!e)
		return (NULL);
	width = fabsf(ft_atof((char *)lst_get(data, 3)));
	return (new_square(width, pos, rot, color));
}

// TODO set all functions not in .h static

int	orient(t_vector3 a, t_vector3 b, t_vector3 c, t_vector3 n)
{
	int	result;

	result = vec3_dotv(vec3_crossv(vec3_subv(b, a), vec3_subv(c, a)), n);
	if (result > 0)
		return (1);
	if (result < 0)
		return (-1);
	return (0);
}

int	collides_square(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*plan;
	t_square	*square;
	int			in;

	plan = arg1;
	ray = arg2;
	square = &(plan->data.square);
	in = orient(ray->phit, square->p1, square->p2, plan->rotation);
	in += orient(ray->phit, square->p2, square->p3, plan->rotation);
	in += orient(ray->phit, square->p3, square->p4, plan->rotation);
	in += orient(ray->phit, square->p4, square->p1, plan->rotation);
	if (in == 4 || in == -4)
		return (1);
	return (0);
}

t_object	*new_square(float width, t_vector3 position, t_vector3 rotation,
	t_color color)
{
	t_object	*plan;
	float		mid;
	t_vector3	diagonal;

	plan = new_default_plane(position, rotation, color, &collides_square);
	if (!plan)
		return (NULL);
	mid = (width * sqrtf(2)) / 2;
	diagonal = vec3_crossv(rotation, vec3_new(1, 0, 0));
	if (vec3_length(diagonal) == 0)
		diagonal = vec3_new(0, 1, 0);
	diagonal = vec3_rotate_axis(diagonal, rotation, M_PI_4);
	diagonal = vec3_muld(vec3_normalize(diagonal), mid);
	plan->data.square.p1 = vec3_addv(diagonal, position);
	plan->data.square.p3 = vec3_addv(vec3_negate(diagonal), position);
	diagonal = vec3_rotate_axis(diagonal, rotation, M_PI_2);
	diagonal = vec3_muld(vec3_normalize(diagonal), mid);
	plan->data.square.p2 = vec3_addv(diagonal, position);
	plan->data.square.p4 = vec3_addv(vec3_negate(diagonal), position);
	plan->data.square.width = width;
	return (plan);
}
