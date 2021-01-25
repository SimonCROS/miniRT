/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/01/25 13:07:11 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_camera	*new_camera(t_vector3 *position, t_vector3 *direction, float fov)
{
	t_camera	*camera;

	if (!(camera = malloc(sizeof(t_camera))))
		return (NULL);
	camera->position = position;
	camera->direction = direction;
	camera->fov = fov;
	return (camera);
}
