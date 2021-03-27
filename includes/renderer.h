#ifndef RENDERER_H
# define RENDERER_H

# include <minirt.h>
# include <pthread.h>

/*** Definitions **************************************************************/

/*** Types ********************************************************************/

typedef struct s_thread_dat	t_thread_data;

/*** General ******************************************************************/

struct s_thread_dat
{
	t_vars			*vars;
	void			*image;
	size_t			width;
	size_t			height;
	t_camera		*camera;
	t_scene			*scene;
	size_t			chunks;
	pthread_mutex_t	mutex_flush;
};

/*** Engine *******************************************************************/

void		*render_thread(t_thread_data *data, int *chunk);

#endif
