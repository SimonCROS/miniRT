#include <math.h>

#include "minirt.h"

int	on_drag(t_vars *vars, t_click_type type)
{
	int			mx;
	int			my;
	t_camera	*c;

	c = vars->camera;
	mx = vars->mouse_x - vars->mouse_x_from;
	my = vars->mouse_y - vars->mouse_y_from;
	if (type == CLICK_MIDDLE)
	{
		c->position = vec3_subv(c->position, vec3_muld(c->right, mx * 0.5));
		c->position = vec3_subv(c->position, vec3_muld(c->up, my * 0.5));
	}
	if (type == CLICK_LEFT)
	{
		c->direction = vec3_rotate_axis(c->direction, c->up,
				M_PI / 800 * -mx);
		c->direction = vec3_rotate_axis(c->direction, c->right,
				M_PI / 800 * my);
		reload_camera(c);
	}
	vars->mouse_x_from = vars->mouse_x;
	vars->mouse_y_from = vars->mouse_y;
	vars->flush = 1;
	return (0);
}
