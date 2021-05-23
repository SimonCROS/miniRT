#include <math.h>

#include "mlx.h"

#include "minirt.h"

static int	init_mlx(t_vars *vars, char *file, t_scene *scene)
{
	char	*name;

	vars->mlx = mlx_init();
	vars->win = NULL;
	if (!vars->mlx)
		return (FALSE);
	init_window_size(vars, scene);
	name = ft_strjoin("MiniRT - ", file);
	if (!name)
		return (FALSE);
	vars->win = mlx_new_window(vars->mlx, scene->render->width,
			scene->render->height, name);
	free(name);
	if (!vars->win)
		return (FALSE);
	mlx_string_put(vars->mlx, vars->win, 10, 50, ~0, "Press 'enter' to start");
	mlx_string_put(vars->mlx, vars->win, 10, 70, ~0, "Press 'm' to \
enable/disable shadows");
	mlx_string_put(vars->mlx, vars->win, 10, 90, ~0, "Press 'n' to \
enable/disable the triangle mode");
	mlx_string_put(vars->mlx, vars->win, 10, 110, ~0, "Press 'c' to \
enable/disable antialiasing (SSAA 4x)");
	mlx_string_put(vars->mlx, vars->win, 10, 130, ~0, "Press 'v' to \
enable/disable normal disruption");
	mlx_string_put(vars->mlx, vars->win, 10, 150, ~0, "Press 'f' to \
enable/disable debug mode");
	mlx_string_put(vars->mlx, vars->win, 10, 170, ~0, "Press 'esc' to exit");
	return (TRUE);
}

void	reset_keys(t_vars *vars)
{
	vars->up = 0;
	vars->down = 0;
	vars->left = 0;
	vars->right = 0;
	vars->cam_up = 0;
	vars->forward = 0;
	vars->backward = 0;
	vars->cam_down = 0;
	vars->cam_left = 0;
	vars->cam_right = 0;
	vars->scroll_direction = 0;
	vars->click_type = CLICK_NONE;
}

void	init_window(char *file, t_scene *scene)
{
	t_vars	vars;

	vars.on_exit = (t_con)mlx_exit;
	vars.scene = scene;
	if (!init_mlx(&vars, file, scene))
	{
		errno = -1;
		perror("Error\nCan't generate the frame");
		exit_minirt(&vars, NULL, NULL, EXIT_FAILURE);
		return ;
	}
	vars.init_image = (t_bifun)mlx_init_image;
	vars.set_pixel = (t_pixel_writer)mlx_set_pixel;
	vars.on_finished = (t_bicon)force_put_image;
	vars.free_image = (t_bicon)mlx_free_image;
	vars.flush = 0;
	vars.camera = NULL;
	launch_game(&vars);
}
