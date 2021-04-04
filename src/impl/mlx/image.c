#include "mlx.h"

#include "minirt.h"

void	mlx_free_image(t_image *image, t_vars *vars)
{
	if (image)
	{
		mlx_destroy_image(vars->mlx, image->img);
		free(image);
	}
}

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
	if (!img->img)
	{
		free(img);
		return (NULL);
	}
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	if (!img->addr)
	{
		mlx_destroy_image(vars->mlx, img->img);
		free(img);
		return (NULL);
	}
	return (img);
}
