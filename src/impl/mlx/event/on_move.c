#include "minirt.h"

int	on_move(t_vars *vars, t_move_type type)
{
	static t_vector3	up = (t_vector3){0, 1, 0};
	t_camera			*cam;

	cam = vars->camera;
	if (type == FORWARD)
		cam->position = vec3_addv(cam->position, vec3_muld(cam->flat, 2));
	else if (type == BACKWARD)
		cam->position = vec3_subv(cam->position, vec3_muld(cam->flat, 2));
	else if (type == LEFT)
		cam->position = vec3_subv(cam->position,
				vec3_muld(vec3_crossv(cam->flat, up), 2));
	else if (type == RIGHT)
		cam->position = vec3_addv(cam->position,
				vec3_muld(vec3_crossv(cam->flat, up), 2));
	else if (type == UP)
		cam->position = vec3_addv(cam->position, vec3_muld(up, 2));
	else if (type == DOWN)
		cam->position = vec3_subv(cam->position, vec3_muld(up, 2));
	vars->flush = 1;
	return (0);
}
