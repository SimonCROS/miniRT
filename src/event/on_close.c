#include "mlx.h"

#include "display/window.h"
#include "provider/scene_provider.h"

int	on_close(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	free_scene(get_scene());
	load_scene(NULL);
	exit(0);
	return (0);
}
