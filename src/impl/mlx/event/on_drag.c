#include "minirt.h"

int	on_drag(t_vars *vars, t_click_type type)
{
	int			move_x;
	int			move_y;
	t_camera	*camera;

	camera = vars->camera;
	move_x = vars->mouse_x - vars->mouse_x_from;
	move_y = vars->mouse_y - vars->mouse_y_from;
	if (type == CLICK_LEFT)
	{
		vars->camera->position = vec3_subv(vars->camera->position,
				vec3_muld(camera->right, move_x * 0.5));
		vars->camera->position = vec3_subv(vars->camera->position,
				vec3_muld(camera->up, move_y * 0.5));
	}
	vars->mouse_x_from = vars->mouse_x;
	vars->mouse_y_from = vars->mouse_y;
	vars->flush = 1;
	return (0);
}
