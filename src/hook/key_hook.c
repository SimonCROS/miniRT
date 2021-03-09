#include "provider/scene_provider.h"
#include "display/window.h"
#include "listener/events.h"
#include "util/logs.h"

int	key_hook(int i, t_vars *vars)
{
	static int	started;
	t_scene		*scene;

	scene = get_scene();
	if (i == 53 || i == 65307)
		return (on_close(vars));
	if (!started && (i == 36 || i == 2))
	{
		started = 1;
		set_debug(i == 2);
		return (on_change_camera(vars));
	}
	else if (scene->cameras->size > 1 && (i == 123 || i == 125 || i == 124
			|| i == 126 || i == 6536 || i == 65362 || i == 65363 || i == 65364))
	{
		if (scene->cameras->size == 1)
			return (0);
		if (i == 123 || i == 125 || i == 65361 || i == 65364)
			scene->index--;
		if (i == 124 || i == 126 || i == 65362 || i == 65363)
			scene->index++;
		scene->index %= scene->cameras->size;
		if (scene->index < 0)
			scene->index = scene->cameras->size + scene->index;
		return (on_change_camera(vars));
	}
	return (0);
}
