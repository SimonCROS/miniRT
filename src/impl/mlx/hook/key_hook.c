#include "minirt.h"

int	key_pressed_hook(int i, t_vars *vars)
{
	static int	started;
	t_scene		*scene;
	t_camera	*camera;

	scene = get_scene();
	if (i == K_ESC)
		exit_minirt(vars, NULL, NULL, EXIT_SUCCESS);
	else if (i == K_F)
		set_debug(!is_debug_enabled());
	else if (!started && (i == K_ENTER))
	{
		started = 1;
		return (on_change_camera(vars));
	}
	else if (started)
	{
		if (i == K_M)
		{
			reset_keys(vars);
			camera = lst_get(scene->cameras, scene->index);
			vars->shadows = !vars->shadows;
			vars->free_image(camera->render, vars);
			free(camera->z_buffer);
			camera->z_buffer = NULL;
			camera->render = NULL;
			return (render(vars));
		}
		else if (scene->cameras->size > 1 && (i == K_O || i == K_P))
		{
			if (i == K_O)
				scene->index--;
			if (i == K_P)
				scene->index++;
			scene->index %= scene->cameras->size;
			if (scene->index < 0)
				scene->index = scene->cameras->size + scene->index;
			return (on_change_camera(vars));
		}
		else if (!vars->shadows && (i == K_UP || i == K_DOWN || i == K_LEFT
				|| i == K_RIGHT || i == K_W || i == K_S || i == K_A || i == K_D
				|| i == K_LSHIFT || i == K_ESP))
		{
			if (i == K_W)
				vars->forward = 1;
			if (i == K_S)
				vars->backward = 1;
			if (i == K_A)
				vars->left = 1;
			if (i == K_D)
				vars->right = 1;
			if (i == K_ESP)
				vars->up = 1;
			if (i == K_LSHIFT)
				vars->down = 1;
			if (i == K_UP)
				vars->cam_up = 1;
			if (i == K_DOWN)
				vars->cam_down = 1;
			if (i == K_LEFT)
				vars->cam_left = 1;
			if (i == K_RIGHT)
				vars->cam_right = 1;
			return (1);
		}
	}
	return (0);
}

int	key_released_hook(int i, t_vars *vars)
{
	if (i == K_UP || i == K_DOWN || i == K_LEFT || i == K_RIGHT
		|| i == K_W || i == K_S || i == K_A || i == K_D
		|| i == K_LSHIFT || i == K_ESP)
	{
		if (i == K_W)
			vars->forward = 0;
		if (i == K_S)
			vars->backward = 0;
		if (i == K_A)
			vars->left = 0;
		if (i == K_D)
			vars->right = 0;
		if (i == K_ESP)
			vars->up = 0;
		if (i == K_LSHIFT)
			vars->down = 0;
		if (i == K_UP)
			vars->cam_up = 0;
		if (i == K_DOWN)
			vars->cam_down = 0;
		if (i == K_LEFT)
			vars->cam_left = 0;
		if (i == K_RIGHT)
			vars->cam_right = 0;
		return (1);
	}
	return (0);
}
