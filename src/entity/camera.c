/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:44:13 by scros             #+#    #+#             */
/*   Updated: 2021/02/22 11:18:11 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_matrix44	look_at(t_vector3 from, t_vector3 to)
{
	static t_vector3	tmp = (t_vector3) { 0, 1, 0 };
	t_matrix44			camToWorld;
    t_vector3			forward;
    t_vector3			right;
    t_vector3			up;

	forward = vec3_normalize(vec3_subv(from, to));
	right = vec3_crossv(tmp, forward);
	if (vec3_length_squared(right) == 0)
		right = vec3_new(1, 0, 0);
	up = vec3_crossv(forward, right);
    camToWorld.e[0][0] = right.x;
    camToWorld.e[0][1] = right.y;
    camToWorld.e[0][2] = right.z;
    camToWorld.e[1][0] = up.x;
    camToWorld.e[1][1] = up.y;
    camToWorld.e[1][2] = up.z;
    camToWorld.e[2][0] = forward.x;
    camToWorld.e[2][1] = forward.y;
    camToWorld.e[2][2] = forward.z;
    camToWorld.e[3][0] = from.x;
    camToWorld.e[3][1] = from.y;
    camToWorld.e[3][2] = from.z;
    return (camToWorld);
}

t_camera	*new_camera(t_vector3 position, t_vector3 direction, float fov)
{
	t_camera	*camera;

	if (!(camera = malloc(sizeof(t_camera))))
		return (NULL);
	camera->position = position;
	camera->direction = vec3_normalize(direction);
	camera->fov = fov;
	camera->c2w = look_at(vec3_new(0, 0, 0), camera->direction);
	camera->render = NULL;
	return (camera);
}
