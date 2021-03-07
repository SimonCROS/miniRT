#ifndef CAMERA_H
# define CAMERA_H

# include "matrix.h"
# include "list.h"

typedef struct s_camera
{
	t_vector3	position;
	t_vector3	direction;
	t_matrix44	c2w;
	void		*render;
	float		fov;
}	t_camera;

t_camera	*parse_camera(t_list *data);
t_camera	*new_camera(t_vector3 position, t_vector3 direction, float fov);

#endif
