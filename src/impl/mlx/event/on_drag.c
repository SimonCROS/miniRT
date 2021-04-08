#include <math.h>

#include "minirt.h"

int	on_drag(t_vars *vars, t_click_type type)
{
	int			move_x;
	int			move_y;
	t_camera	*camera;

	camera = vars->camera;
	move_x = vars->mouse_x - vars->mouse_x_from;
	move_y = vars->mouse_y - vars->mouse_y_from;
	if (type == CLICK_MIDDLE)
	{
		camera->position = vec3_subv(camera->position,
				vec3_muld(camera->right, move_x * 0.5));
		camera->position = vec3_subv(camera->position,
				vec3_muld(camera->up, move_y * 0.5));
	}
	if (type == CLICK_LEFT)
	{
		camera->direction = vec3_rotate_axis(camera->direction, camera->up,
				M_PI / 300 * move_x);
		camera->direction = vec3_rotate_axis(camera->direction, camera->right,
				M_PI / 300 * -move_y);
		reload_camera(camera);
	}
	vars->mouse_x_from = vars->mouse_x;
	vars->mouse_y_from = vars->mouse_y;
	vars->flush = 1;
	return (0);
}
