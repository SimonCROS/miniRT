#include "mlx.h"

#include "minirt.h"

void	apply_sepia(t_options *options, char *image_addr, int bits_per_pixel)
{
	unsigned int	i;
	unsigned int	*dst;
	int				bytes_per_pixel;

	i = 0;
	bytes_per_pixel = bits_per_pixel / 8;
	while (i < options->width * options->height)
	{
		image_addr += bytes_per_pixel;
		dst = (unsigned int *)image_addr;
		*dst = color_to_hex(to_sepia(color_from_hex(*dst)));
	}
}

#if defined __APPLE__

void	force_put_image(t_vars *vars, t_image *image)
{
	if (vars->camera->sepia)
		apply_sepia(get_scene()->render, image->addr, image->bits_per_pixel);
	mlx_sync(MLX_SYNC_IMAGE_WRITABLE, image->img);
	mlx_sync(MLX_SYNC_WIN_FLUSH_CMD, vars->win);
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
	mlx_sync(MLX_SYNC_WIN_CMD_COMPLETED, vars->win);
}

#elif defined __linux__

void	force_put_image(t_vars *vars, t_image *image)
{
	if (vars->camera->sepia)
		apply_sepia(get_scene()->render, image->addr, image->bits_per_pixel);
	*(int *)(vars->mlx + 80) = 1;
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
}

#endif
