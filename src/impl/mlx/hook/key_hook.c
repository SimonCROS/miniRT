#include "minirt.h"

static void	change_camera(t_vars *vars, t_camera *camera, int i, t_scene *scene)
{
	if (i == K_O)
		scene->index--;
	if (i == K_P)
		scene->index++;
	scene->index %= scene->cameras->size;
	if (scene->index < 0)
		scene->index = scene->cameras->size + scene->index;
	vars->camera = (t_camera *)lst_get(scene->cameras, scene->index);
	on_change_camera(vars, camera, vars->camera);
}

static void	toggle_option(t_vars *vars, t_camera *camera, int i, t_scene *scene)
{
	if (i == K_M)
		camera->shadows = !camera->shadows;
	else if (i == K_V)
		camera->normal_disruption = !camera->normal_disruption;
	else if (i == K_U)
		camera->sphere_up_map = !camera->sphere_up_map;
	else if (i == K_B)
		camera->color_disruption = !camera->color_disruption;
	else if (i == K_N)
		camera->show_triangles = !camera->show_triangles;
	else if (i == K_X)
		camera->sepia = !camera->sepia;
	else if (i == K_C)
	{
		if (scene->triangles->size)
		{
			log_msg(WARN, "Antialiasing can't be enabled with triangles.");
			return ;
		}
		if (scene->render->samples == 1)
			scene->render->samples = 4;
		else
			scene->render->samples = 1;
	}
	vars->flush = 1;
}

static void	move_pressed(t_vars *vars, t_camera *camera, int i)
{
	if (camera->shadows)
		log_msg(ERROR, "You must disable shadows to move.");
	else if (i == K_W)
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
	else if (i == K_LEFT)
		vars->cam_left = 1;
	else if (i == K_RIGHT)
		vars->cam_right = 1;
	else if (i == K_UP)
		vars->cam_up = 1;
	else if (i == K_DOWN)
		vars->cam_down = 1;
}

int	key_pressed_hook(int i, t_vars *vars)
{
	t_scene		*scene;

	scene = vars->scene;
	if (i == K_ESC)
		exit_minirt(vars, NULL, NULL, EXIT_SUCCESS);
	else if (i == K_F)
		set_debug(!is_debug_enabled());
	else if (!vars->camera && i == K_ENTER)
	{
		vars->camera = (t_camera *)lst_first(scene->cameras);
		return (on_change_camera(vars, NULL, vars->camera));
	}
	else if (vars->camera)
	{
		if (scene->cameras->size > 1 && (i == K_O || i == K_P))
			change_camera(vars, vars->camera, i, scene);
		else if (i == K_W || i == K_S || i == K_A || i == K_D || i == K_ESP
			|| i == K_LSHIFT || i == K_LEFT || i == K_RIGHT || i == K_UP
			|| i == K_DOWN)
			move_pressed(vars, vars->camera, i);
		else if (i == K_B || i == K_C || i == K_U || i == K_V || i == K_X
			|| i == K_M || i == K_N)
			toggle_option(vars, vars->camera, i, scene);
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
	else if (i == K_LEFT)
		vars->cam_left = 0;
	else if (i == K_RIGHT)
		vars->cam_right = 0;
	else if (i == K_UP)
		vars->cam_up = 0;
	else if (i == K_DOWN)
		vars->cam_down = 0;
	return (0);
}
