/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 15:36:54 by scros             #+#    #+#             */
/*   Updated: 2021/01/16 15:47:42 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

# include "vector3.h"
# include "color.h"

typedef struct	s_light
{
	float		brightness;
	t_vector3	*position;
	t_color		*color;
}				t_light;

t_light			*new_light(float brightness, t_vector3 *position, t_color *color);

#endif
