#include "mlx.h"

#include "minirt.h"

#if defined __APPLE__

static void	mlx_exit(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	free(vars->mlx);
}

#elif defined __linux__

static void	mlx_exit(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
}

#endif

static void	init_window_size(t_vars *vars, t_scene *scene)
{
	int		max_width;
	int		max_height;

	mlx_get_screen_size(vars->mlx, &max_width, &max_height);
	if (max_width > 0 && scene->render->width > (size_t)max_width)
	{
		scene->render->width = (size_t)max_width;
		log_msg(WARN, "Width too long, using the window width.");
	}
	if (max_height > 0 && scene->render->height > (size_t)max_height)
	{
		scene->render->height = (size_t)max_height;
		log_msg(WARN, "Height too long, using the window height.");
	}
}

static int	init_mlx(t_vars *vars, char *file, t_scene *scene)
{
	char	*name;

	vars->mlx = mlx_init();
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
		errno = -1;
		perror("Error\nCan't generate the frame");
		exit(EXIT_FAILURE);
		return ;
	}
	vars.init_image = (t_bifun)mlx_init_image;
	vars.set_pixel = (t_pixel_writer)mlx_set_pixel;
	vars.on_refresh = (t_bicon)force_put_image;
	vars.on_finished = null_biconsumer();
	vars.free_image = (t_bicon)mlx_free_image;
	vars.on_exit = (t_con)mlx_exit;
	mlx_hook(vars.win, 17, 0L, &close_hook, &vars);
	mlx_key_hook(vars.win, &key_hook, &vars);
	mlx_loop(vars.mlx);
}
