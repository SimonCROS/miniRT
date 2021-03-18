#ifndef WINDOW_H
# define WINDOW_H

# include <stdlib.h>

#include "libft.h"

# include "util/scene.h"

# ifdef __APPLE__
#  define K_LEFT 123
#  define K_RIGHT 124
#  define K_DOWN 125
#  define K_UP 126
#  define K_ESC 53
#  define K_ENTER 36
#  define K_D 2
# else
#  define K_LEFT 65361
#  define K_RIGHT 65364
#  define K_DOWN 65362
#  define K_UP 65362
#  define K_ESC 65307
#  define K_ENTER 65293
#  define K_D 100
# endif

typedef void	(*t_pixel_writer)(void *, int, int, t_color);

typedef struct s_vars
{
	void			*mlx;
	void			*win;
	t_bifunction	init_image;
	t_pixel_writer	set_pixel;
	t_biconsumer	on_refresh;
	t_biconsumer	on_finished;
	t_biconsumer	on_state_change;
}	t_vars;

void	init_window(char *file, t_scene *scene);

#endif
