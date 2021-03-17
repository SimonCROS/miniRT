#include "mlx.h"

#include "display/window.h"
#include "provider/scene_provider.h"
#include "display/image.h"

#ifdef __linux__
# include <X11/Xlib.h>
#endif

static void	free_render(t_image *image, t_vars *vars)
{
	if (image)
	{
		mlx_destroy_image(vars->mlx, image->img);
		free(image);
	}
}

int	on_close(t_vars *vars)
{
	lst_foreachp(get_scene()->cameras, (t_bicon)free_render, vars);
	free_scene(get_scene());
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	exit(0);
	return (0);
}
