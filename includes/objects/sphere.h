/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 18:46:03 by scros             #+#    #+#             */
/*   Updated: 2021/01/17 16:07:00 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
# define SPHERE_H

# include "vector3.h"
# include "color.h"

typedef struct	s_sphere
{
	float		radius;
	t_vector3	*position;
	t_color		*color;
}				t_sphere;

t_sphere		*new_sphere(float radius, t_vector3 *position, t_color *color);
short			sphere_collision(t_sphere *sphere, t_vector3 *origin, t_vector3 *dir, float *t0, float *t1);

#endif
