#include "minirt.h"

int	on_scroll(t_vars *vars, int direction)
{
	t_camera	*camera;

	camera = vars->camera;
	camera->position = vec3_addv(camera->position,
			vec3_muld(camera->direction, direction * 10));
	vars->flush = 1;
	return (0);
}
