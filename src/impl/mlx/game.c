#include <math.h>

#include "mlx.h"

#include "minirt.h"

static void	compute_move_keys_flat(t_vars *vars)
{
	static t_vec3f	up = (t_vec3f){0, 1, 0};
	t_camera		*c;

	c = vars->camera;
	if (vars->up)
		c->position = vec3_add(c->position, vec3_muld(up, 2));
	if (vars->down)
		c->position = vec3_sub(c->position, vec3_muld(up, 2));
	if (vars->forward)
		c->position = vec3_add(c->position, vec3_muld(c->flat, 2));
	if (vars->backward)
		c->position = vec3_sub(c->position, vec3_muld(c->flat, 2));
	if (vars->left)
		c->position = vec3_sub(c->position,
				vec3_muld(vec3_cross(c->flat, up), 2));
	if (vars->right)
		c->position = vec3_add(c->position,
				vec3_muld(vec3_cross(c->flat, up), 2));
}

static void	compute_move_keys(t_vars *vars)
{
	t_camera		*c;

	c = vars->camera;
	compute_move_keys_flat(vars);
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

static int	loop(t_vars *vars)
{
	t_camera	*camera;

	camera = vars->camera;
	if (!camera)
		return (0);
	compute_move_keys(vars);
	if (!vars->flush)
		return (0);
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

void	launch_game(t_vars *vars)
{
	reset_keys(vars);
	mlx_hook(vars->win, 17, 0L, close_hook, vars);
	mlx_hook(vars->win, 2, 1L << 0, key_pressed_hook, vars);
	mlx_hook(vars->win, 3, 1L << 1, key_released_hook, vars);
	mlx_hook(vars->win, 4, 1L << 2, mouse_pressed_hook, vars);
	mlx_hook(vars->win, 5, 1L << 3, mouse_released_hook, vars);
	mlx_hook(vars->win, 6, 1L << 6, mouse_moved_hook, vars);
	mlx_loop_hook(vars->mlx, (t_pre)loop, vars);
	mlx_loop(vars->mlx);
}
