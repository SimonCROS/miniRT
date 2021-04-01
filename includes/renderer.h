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
	size_t			width;
	size_t			height;
	t_camera		*camera;
	t_scene			*scene;
	size_t			chunks;
	pthread_mutex_t	mutex_flush;
};

/*** Engine *******************************************************************/

void		*render_thread(t_thread_data *data, int *chunk);

/*** Z-Buffer *****************************************************************/

void		fill_z_buff(float *z_buffer, size_t length);
float		*get_z_buffer_value(float *z_buf, size_t x, size_t y, size_t width);

#endif
