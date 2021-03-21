#include "mlx.h"

#include "minirt.h"

#if defined __APPLE__

void	force_put_image(t_vars *vars, t_image *image)
{
	mlx_sync(MLX_SYNC_IMAGE_WRITABLE, image->img);
	mlx_sync(MLX_SYNC_WIN_FLUSH_CMD, vars->win);
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
	mlx_sync(MLX_SYNC_WIN_CMD_COMPLETED, vars->win);
}

#elif defined __linux__

void	force_put_image(t_vars *vars, t_image *image)
{
	*(int *)(vars->mlx + 80) = 1;
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
}

#endif
