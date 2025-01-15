#include "mlx.h"

#include "minirt.h"

#if defined __APPLE__

void	mlx_exit(t_vars *vars)
{
	if (vars->mlx)
		if (vars->win)
			mlx_destroy_window(vars->mlx, vars->win);
}

#elif defined __linux__

void	mlx_exit(t_vars *vars)
{
	if (vars->mlx)
	{
		if (vars->win)
			mlx_destroy_window(vars->mlx, vars->win);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
	}
}

#endif

void	init_window_size(t_vars *vars, t_scene *scene)
{
	int		max_width;
	int		max_height;

#if defined __APPLE__
	(void)vars;
	max_width = 4200;
	max_height = 2400;
#else
	mlx_get_screen_size(vars->mlx, &max_width, &max_height);
#endif
	if (max_width > 0 && scene->render->width > max_width)
	{
		scene->render->width = max_width;
		log_msg(WARN, "Width too long, using the window width.");
	}
	if (max_height > 0 && scene->render->height > max_height)
	{
		scene->render->height = max_height;
		log_msg(WARN, "Height too long, using the window height.");
	}
}
