#ifndef WINDOW_H
# define WINDOW_H

# include <stdlib.h>

# include "types.h"
# include "color.h"

# include "util/scene.h"

typedef void	(*t_pixel_writer)(void *, uint32_t, uint32_t, t_color);

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
