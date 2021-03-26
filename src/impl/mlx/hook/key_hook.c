#include "minirt.h"

int	key_hook(int i, t_vars *vars)
{
	static int	started;
	t_scene		*scene;

	scene = get_scene();
	if (i == K_ESC)
		exit_minirt(vars, NULL, NULL, EXIT_SUCCESS);
	if (!started && (i == K_ENTER || i == K_D))
	{
		started = 1;
		set_debug(i == 2);
		return (on_change_camera(vars));
	}
	else if (scene->cameras->size > 1 && (i == K_LEFT || i == K_RIGHT
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
	return (0);
}
