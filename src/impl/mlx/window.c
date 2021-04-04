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
	mlx_string_put(vars->mlx, vars->win, 0, 70, ~0, "Press 'f' to \
enable/disable debug mode");
	mlx_string_put(vars->mlx, vars->win, 0, 90, ~0, "Press 'esc' to exit");
	return (TRUE);
}

int	loop(t_vars *vars)
{
	static t_vector3	up = (t_vector3){0, 1, 0};
	t_camera			*camera;
	t_vector3			flat_direction;
	t_vector3			right_direction;

	if (!vars->forward && !vars->backward && !vars->left && !vars->right
		&& !vars->up && !vars->down && !vars->cam_left
		&& !vars->cam_right && !vars->cam_down && !vars->cam_up)
		return (0);
	camera = lst_get(get_scene()->cameras, get_scene()->index);

	vars->free_image(camera->render, vars);
	free(camera->z_buffer);
	camera->z_buffer = NULL;
	camera->render = NULL;

	flat_direction = camera->direction;
	flat_direction.y = 0;
	flat_direction = vec3_normalize(flat_direction);
	right_direction = vec3_crossv(up, flat_direction);
	if (!vec3_length_squared(flat_direction))
		flat_direction = vec3_new(0, 0, 1);
	if (!vec3_length_squared(right_direction))
		right_direction = vec3_new(1, 0, 0);
	if (vars->forward)
		camera->position = vec3_addv(camera->position, vec3_muld(flat_direction, 2));
	if (vars->backward)
		camera->position = vec3_subv(camera->position, vec3_muld(flat_direction, 2));
	if (vars->left)
		camera->position = vec3_subv(camera->position, vec3_muld(vec3_crossv(flat_direction, up), 2));
	if (vars->right)
		camera->position = vec3_addv(camera->position, vec3_muld(vec3_crossv(flat_direction, up), 2));
	if (vars->up)
		camera->position = vec3_addv(camera->position, vec3_muld(up, 2));
	if (vars->down)
		camera->position = vec3_subv(camera->position, vec3_muld(up, 2));
	if (vars->cam_left)
		camera->direction = vec3_rotate_y(camera->direction, M_PI / 30);
	if (vars->cam_right)
		camera->direction = vec3_rotate_y(camera->direction, -M_PI / 30);
	if (vars->cam_up)
		camera->direction = vec3_rotate_axis(camera->direction, right_direction, -M_PI / 15);
	if (vars->cam_down)
		camera->direction = vec3_rotate_axis(camera->direction, right_direction, M_PI / 15);
	if (vars->cam_left || vars->cam_right || vars->cam_up || vars->cam_down)
		reload_camera(camera);
	if (log_msg(DEBUG, NULL))
	{
		printf("Camera position set to %.2f,%.2f,%.2f, direction set to \
%.2f,%.2f,%.2f", camera->position.x, camera->position.y, camera->position.z,
			camera->direction.x, camera->direction.y, camera->direction.z);
		log_nl();
	}
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
	vars.forward = 0;
	vars.backward = 0;
	vars.left = 0;
	vars.right = 0;
	vars.up = 0;
	vars.down = 0;
	vars.cam_left = 0;
	vars.cam_right = 0;
	vars.cam_up = 0;
	vars.cam_down = 0;
	vars.init_image = (t_bifun)mlx_init_image;
	vars.set_pixel = (t_pixel_writer)mlx_set_pixel;
	vars.on_refresh = (t_bicon)force_put_image;
	vars.on_finished = null_biconsumer();
	vars.free_image = (t_bicon)mlx_free_image;
	mlx_hook(vars.win, 17, 0L, close_hook, &vars);
	mlx_hook(vars.win, 2, 1L << 0, key_pressed_hook, &vars);
	mlx_hook(vars.win, 3, 1L << 1, key_released_hook, &vars);
	mlx_loop_hook(vars.mlx, (t_pre)loop, &vars);
	mlx_loop(vars.mlx);
}
