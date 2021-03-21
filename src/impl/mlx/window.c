#include <stdio.h>

#include "mlx.h"

#include "minirt.h"

static int	init_mlx(t_vars *vars, char *file, t_scene *scene)
{
	char	*name;

	vars->mlx = mlx_init();
	if (!vars->mlx)
		return (FALSE);
	name = ft_strjoin("MiniRT - ", file);
	if (!name)
		return (FALSE);
	vars->win = mlx_new_window(vars->mlx, scene->render->width,
			scene->render->height, name);
	free(name);
	if (!vars->win)
		return (FALSE);
	mlx_string_put(vars->mlx, vars->win, 0, 50, ~0, "Press 'enter' to start");
	mlx_string_put(vars->mlx, vars->win, 0, 70, ~0, "Press 'd' to start in \
debug mode");
	mlx_string_put(vars->mlx, vars->win, 0, 90, ~0, "Press 'esc' to exit");
	return (TRUE);
}

void	init_window(char *file, t_scene *scene)
{
	t_vars	vars;

	if (!init_mlx(&vars, file, scene))
	{
		free_scene(scene);
		perror("Error\nCan't generate the frame");
		exit(EXIT_FAILURE);
		return ;
	}
	vars.init_image = (t_bifun)mlx_init_image;
	vars.set_pixel = (t_pixel_writer)mlx_set_pixel;
	vars.on_refresh = (t_bicon)force_put_image;
	vars.on_finished = null_biconsumer();
	mlx_hook(vars.win, 17, 0L, &close_hook, &vars);
	mlx_key_hook(vars.win, &key_hook, &vars);
	mlx_loop(vars.mlx);
}
