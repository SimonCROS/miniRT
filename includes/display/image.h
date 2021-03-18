#ifndef IMAGE_H
# define IMAGE_H

#include "libft.h"

# include "display/window.h"
# include "options.h"

typedef struct s_image
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}	t_image;

t_image	*mlx_init_image(t_vars *vars, t_options *params);
void	mlx_set_pixel(t_image *image, int x, int y, t_color color);
void	force_put_image(t_vars *vars, t_image *image);

#endif
