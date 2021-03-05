#ifndef MINIRT_H
# define MINIRT_H

# define MAX_THREADS 16

# define BUFFER_SIZE 50

# include <math.h>
# include "libft.h"
# include "bitmap.h"
# include "mlx.h"

typedef struct		s_image {
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}					t_image;

typedef struct		s_ray {
	t_vector3		direction;
	t_vector3		phit;
	t_vector3		nhit;
	t_vector3		origin;
	t_color			color;
	float			length;
	float			light;
}					t_ray;

typedef	void		(*t_pixel_writer)(void *, uint32_t, uint32_t, t_color);

typedef struct		s_vars {
	void			*mlx;
	void			*win;
	t_bifunction	init_image;
	t_pixel_writer	set_pixel;
	t_biconsumer	on_refresh;
	t_biconsumer	on_finished;
}					t_vars;

typedef struct		s_render_params {
	int				width;
	int				height;
	int				threads;
	int				chunk_width;
	int				chunk_height;
}					t_render_params;

typedef struct		s_scene {
	t_render_params	*render;
	t_list			*cameras;
	t_list			*lights;
	t_list			*objects;
	t_color			*ambiant;
	int				index;
}					t_scene;

# include "object.h"

typedef struct		s_thread_data
{
	t_vars			*vars;
	void			*image;
	size_t			width;
	size_t			height;
	t_camera		*camera;
	t_scene			*scene;
	size_t			chunks;
}					t_thread_data;

t_scene				*parse_file(char *file);
void				set_pixel(t_image *data, int x, int y, int color);

#endif
