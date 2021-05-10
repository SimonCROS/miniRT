#include "mlx.h"

#include "minirt.h"
#include <math.h>

#if defined __APPLE__

static void	mlx_exit(t_vars *vars)
{
	if (vars->mlx)
		if (vars->win)
			mlx_destroy_window(vars->mlx, vars->win);
}

#elif defined __linux__

static void	mlx_exit(t_vars *vars)
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
	vars->samples = 2;
	vars->click_type = CLICK_NONE;
}

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
	mlx_string_put(vars->mlx, vars->win, 0, 50, ~0, "Press 'enter' to start");
	mlx_string_put(vars->mlx, vars->win, 0, 70, ~0, "Press 'm' to \
enable/disable shadows");
	mlx_string_put(vars->mlx, vars->win, 0, 90, ~0, "Press 'm' to \
enable/disable the triangle mode");
	mlx_string_put(vars->mlx, vars->win, 0, 110, ~0, "Press 'f' to \
enable/disable debug mode");
	mlx_string_put(vars->mlx, vars->win, 0, 130, ~0, "Press 'esc' to exit");
	return (TRUE);
}

void	launch_move_events(t_vars *vars)
{
	static t_vector3	up = (t_vector3){0, 1, 0};
	t_camera			*c;

	c = vars->camera;
	if (vars->up)
		c->position = vec3_addv(c->position, vec3_muld(up, 2));
	if (vars->down)
		c->position = vec3_subv(c->position, vec3_muld(up, 2));
	if (vars->forward)
		c->position = vec3_addv(c->position, vec3_muld(c->flat, 2));
	if (vars->backward)
		c->position = vec3_subv(c->position, vec3_muld(c->flat, 2));
	if (vars->left)
		c->position = vec3_subv(c->position,
				vec3_muld(vec3_crossv(c->flat, up), 2));
	if (vars->right)
		c->position = vec3_addv(c->position,
				vec3_muld(vec3_crossv(c->flat, up), 2));
	if (vars->cam_left)
		c->direction = vec3_rotate_y(c->direction, M_PI / 30);
	if (vars->cam_right)
		c->direction = vec3_rotate_y(c->direction, -M_PI / 30);
	if (vars->cam_up)
		c->direction = vec3_rotate_axis(c->direction, c->right, M_PI / 15);
	if (vars->cam_down)
		c->direction = vec3_rotate_axis(c->direction, c->right, -M_PI / 15);
	if (vars->cam_left || vars->cam_right || vars->cam_up || vars->cam_down)
		reload_camera(c);
	if (vars->up || vars->down || vars->left || vars->right || vars->forward
		|| vars->backward || vars->cam_left || vars->cam_right
		|| vars->cam_up || vars->cam_down)
		vars->flush = 1;
}

int	loop(t_vars *vars)
{
	t_camera			*camera;

	camera = vars->camera;
	launch_move_events(vars);
	if (!vars->flush)
		return (0);
	vars->free_image(camera->render, vars);
	free(camera->z_buffer);
	camera->render = NULL;
	camera->z_buffer = NULL;
	if (log_msg(DEBUG, NULL))
	{
		printf("Camera position set to %.2f,%.2f,%.2f, direction set to \
%.2f,%.2f,%.2f", camera->position.x, camera->position.y, camera->position.z,
			camera->direction.x, camera->direction.y, camera->direction.z);
		log_nl();
	}
	vars->flush = 0;
	return (render(vars));
}

void	init_window(char *file, t_scene *scene)
{
	t_vars	vars;

	vars.on_exit = (t_con)mlx_exit;
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
	reset_keys(&vars);
	mlx_hook(vars.win, 17, 0L, close_hook, &vars);
	mlx_hook(vars.win, 2, 1L << 0, key_pressed_hook, &vars);
	mlx_hook(vars.win, 3, 1L << 1, key_released_hook, &vars);
	mlx_hook(vars.win, 4, 1L << 2, mouse_pressed_hook, &vars);
	mlx_hook(vars.win, 5, 1L << 3, mouse_released_hook, &vars);
	mlx_hook(vars.win, 6, 1L << 6, mouse_moved_hook, &vars);
	mlx_loop_hook(vars.mlx, (t_pre)loop, &vars);
	mlx_loop(vars.mlx);
}
