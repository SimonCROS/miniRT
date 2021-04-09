#include "minirt.h"

int	mouse_pressed_hook(int b, int x, int y, t_vars *vars)
{
	if (!vars->camera || vars->camera->shadows)
		return (0);
	if (b > 0 && b < 4 && vars->click_type == CLICK_NONE)
	{
		vars->click_type = b;
		vars->mouse_x_from = x;
		vars->mouse_y_from = y;
		on_click(vars, vars->click_type);
	}
	else
	{
		if (b == SCROLL_DOWN)
			vars->scroll_direction = -1;
		else if (b == SCROLL_UP)
			vars->scroll_direction = 1;
		on_scroll(vars, vars->scroll_direction);
	}
	return (1);
}

int	mouse_moved_hook(int x, int y, t_vars *vars)
{
	if (!vars->camera || vars->camera->shadows)
		return (0);
	vars->mouse_x = x;
	vars->mouse_y = y;
	if (vars->click_type != CLICK_NONE)
		on_drag(vars, vars->click_type);
	return (1);
}

int	mouse_released_hook(int b, int x, int y, t_vars *vars)
{
	if (!vars->camera || vars->camera->shadows)
		return (0);
	(void)x;
	(void)y;
	if (b > 0 && b < 4 && b == vars->click_type)
	{
		on_release_click(vars, vars->click_type);
		vars->mouse_x_from = 0;
		vars->mouse_y_from = 0;
		vars->click_type = CLICK_NONE;
	}
	return (1);
}
