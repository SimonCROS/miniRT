#ifndef RENDER_THREAD_H
# define RENDER_THREAD_H

# include "element/camera.h"
# include "display/window.h"
# include "util/scene.h"

typedef struct s_thread_data
{
	t_vars			*vars;
	void			*image;
	size_t			width;
	size_t			height;
	t_camera		*camera;
	t_scene			*scene;
	size_t			chunks;
}	t_thread_data;

#endif
