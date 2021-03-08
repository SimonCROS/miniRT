#include "mlx.h"

#include "display/image.h"
#include "engine/render_thread.h"

void	mlx_set_pixel(t_image *data, int x, int y, t_color color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color_to_hex(color);
}

t_image	*mlx_init_image(t_vars *vars, t_options *params)
{
	t_image	*img;

	img = malloc(sizeof(t_image));
	if (!img)
		return (NULL);
	img->img = mlx_new_image(vars->mlx, params->width, params->height);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	return (img);
}

void	force_put_image(t_vars *vars, t_image *image)
{
#if defined __APPLE__
	mlx_sync(MLX_SYNC_WIN_FLUSH_CMD, vars->win);
#elif defined __linux__
	*(int*)(vars->mlx + 80) = 1;
#endif
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
}
