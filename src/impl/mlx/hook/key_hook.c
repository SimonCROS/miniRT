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
		vars->camera = (t_camera *)lst_first(scene->cameras);
		return (on_change_camera(vars));
	}
	else if (started)
	{
		camera = vars->camera;
		if (i == K_M)
		{
			if (camera->show_triangles)
			{
				log_msg(ERROR, "You must disable the triangle mode to enable \
shadows.");
				return (0);
			}
			reset_keys(vars);
			camera->shadows = !camera->shadows;
			vars->flush = 1;
			return (render(vars));
		}
		else if (i == K_N)
		{
			if (camera->shadows)
			{
				log_msg(ERROR, "You must disable shadows to enable the \
triangle mode.");
				return (0);
			}
			camera->show_triangles = !camera->show_triangles;
			vars->flush = 1;
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
			vars->camera = (t_camera*)lst_get(scene->cameras, scene->index);
			return (on_change_camera(vars));
		}
		else if (i == K_W || i == K_S || i == K_A || i == K_D || i == K_ESP
			|| i == K_LSHIFT)
		{
			if (camera->shadows)
			{
				log_msg(ERROR, "You must disable shadows to move.");
				return (0);
			}
			if (i == K_W)
				vars->forward = 1;
			else if (i == K_S)
				vars->backward = 1;
			else if (i == K_A)
				vars->left = 1;
			else if (i == K_D)
				vars->right = 1;
			else if (i == K_ESP)
				vars->up = 1;
			else if (i == K_LSHIFT)
				vars->down = 1;
		}
	}
	return (0);
}

int	key_released_hook(int i, t_vars *vars)
{
	if (i == K_W)
		vars->forward = 0;
	else if (i == K_S)
		vars->backward = 0;
	else if (i == K_A)
		vars->left = 0;
	else if (i == K_D)
		vars->right = 0;
	else if (i == K_ESP)
		vars->up = 0;
	else if (i == K_LSHIFT)
		vars->down = 0;
	return (0);
}
