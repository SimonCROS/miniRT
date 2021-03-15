#ifndef SCENE_H
# define SCENE_H

# include "list.h"
# include "color.h"

# include "options.h"

typedef struct s_scene {
	t_options	*render;
	t_list		*cameras;
	t_list		*lights;
	t_list		*objects;
	t_color		*ambiant;
	t_color		*background;
	int			index;
}	t_scene;

#endif
