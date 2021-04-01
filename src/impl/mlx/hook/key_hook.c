#include "minirt.h"

int	key_pressed_hook(int i, t_vars *vars)
{
	static int			started;
	t_scene				*scene;

	printf("%d\n", i);
	scene = get_scene();
	if (i == K_ESC)
		exit_minirt(vars, NULL, NULL, EXIT_SUCCESS);
	else if (i == K_ESP)
		set_debug(!is_debug_enabled());
	else if (!started && (i == K_ENTER))
	{
		started = 1;
		return (on_change_camera(vars));
	}
	else if (started)
	{
		if (scene->cameras->size > 1 && (i == K_LEFT || i == K_RIGHT
				|| i == K_UP || i == K_DOWN))
		{
			if (i == K_LEFT || i == K_DOWN)
				scene->index--;
			if (i == K_RIGHT || i == K_UP)
				scene->index++;
			scene->index %= scene->cameras->size;
			if (scene->index < 0)
				scene->index = scene->cameras->size + scene->index;
			return (on_change_camera(vars));
		}
		else if (i == K_W || i == K_S || i == K_A || i == K_D)
		{
			if (i == K_W)
				vars->forward = 1;
			if (i == K_S)
				vars->backward = 1;
			if (i == K_A)
				vars->left = 1;
			if (i == K_D)
				vars->right = 1;
			return (1);
		}
	}
	return (0);
}

int	key_released_hook(int i, t_vars *vars)
{
	if (i == K_W || i == K_S || i == K_A || i == K_D)
	{
		if (i == K_W)
			vars->forward = 0;
		if (i == K_S)
			vars->backward = 0;
		if (i == K_A)
			vars->left = 0;
		if (i == K_D)
			vars->right = 0;
		return (1);
	}
	return (0);
}
