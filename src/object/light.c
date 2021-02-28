/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/17 15:35:40 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_light	*parse_light(t_list *data)
{
	t_vector3	pos;
	float		brightness;
	t_color		color;
	int			e;

	if (data->size != 4)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && is_float((char *)lst_get(data, 2));
	e = e && color_deserialize((char *)lst_get(data, 3), &color);
	if (!e)
		return (NULL);
	brightness = ft_atof((char *)lst_get(data, 2));
	if (brightness < 0 || brightness > 1)
		return (NULL);
	return (new_light(brightness, pos, color));
}

t_light	*new_light(float brightness, t_vector3 position, t_color color)
{
	t_light	*light;

	if (!(light = malloc(sizeof(t_light))))
		return (NULL);
	light->brightness = brightness;
	light->position = position;
	light->color = color;
	return (light);
}
