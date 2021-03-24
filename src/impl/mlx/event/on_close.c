#include "mlx.h"

#include "minirt.h"

static void	free_render(t_camera *camera, t_vars *vars)
{
	t_image	*image;

	image = (t_image *)camera->render;
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
	exit(0);
	return (0);
}
