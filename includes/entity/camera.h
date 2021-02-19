/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 15:36:54 by scros             #+#    #+#             */
/*   Updated: 2021/01/16 15:47:42 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "matrix.h"

typedef struct	s_camera
{
	t_vector3	position;
	t_vector3	direction;
	t_matrix44	c2w;
	void		*render;
	float		fov;
}				t_camera;

t_camera		*new_camera(t_vector3 position, t_vector3 direction, float fov);

#endif
