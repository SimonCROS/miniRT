#ifndef RENDERER_H
# define RENDERER_H

# include "minirt.h"

# include <pthread.h>

/*** Definitions **************************************************************/

/*** Types ********************************************************************/

typedef struct s_thread_dat	t_thread_data;

/*** General ******************************************************************/

struct s_thread_dat
{
	t_vars			*vars;
	int				width;
	int				height;
	t_camera		*camera;
	t_scene			*scene;
	int				chunks;
};

/*** Engine *******************************************************************/

void		*object_thread(t_thread_data *data, int *chunk);

/*** Z-Buffer *****************************************************************/

void		fill_z_buff(float *z_buffer, int length);
float		*get_z_buffer_value(float *z_buf, int x, int y, int width);

#endif
