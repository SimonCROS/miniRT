#ifndef MINIRT_H
# define MINIRT_H

# include <errno.h>
# include <stdio.h>
# include <pthread.h>

# include "libft.h"

/*** Definitions **************************************************************/

# define MAX_THREADS 16
# define BUFFER_SIZE 50

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

/*** Types ********************************************************************/

typedef struct s_vars		t_vars;

typedef enum e_log_type		t_log_type;

typedef struct s_scene		t_scene;
typedef struct s_options	t_options;

typedef struct s_image		t_image;
typedef void				(*t_pixel_writer)(void *, int, int, t_color);

typedef struct s_ray		t_ray;
typedef struct s_thread_dat	t_thread_data;
typedef struct s_camera		t_camera;

/*** General ******************************************************************/

struct s_vars
{
	void			*mlx;
	void			*win;
	t_bifunction	init_image;
	t_pixel_writer	set_pixel;
	t_biconsumer	on_refresh;
	t_biconsumer	on_finished;
	t_biconsumer	on_state_change;
};

struct s_thread_dat
{
	t_vars			*vars;
	void			*image;
	size_t			width;
	size_t			height;
	t_camera		*camera;
	t_scene			*scene;
	size_t			chunks;
};

/*** Events *******************************************************************/

int			on_change_camera(t_vars *vars);
int			on_close(t_vars *vars);

/*** Hooks ********************************************************************/

int			key_hook(int key, t_vars *vars, t_scene *scene);
int			close_hook(t_vars *vars, t_scene *scene);

/*** Engine *******************************************************************/

struct s_ray
{
	t_vector3	direction;
	t_vector3	phit;
	t_vector3	nhit;
	t_vector3	origin;
	t_color		color;
	float		length;
	float		light;
};

t_ray		compute_ray(t_options *render, t_camera *camera, float x, float y);
int			collision(t_object *object, t_ray *ray);
int			intersect_plane(t_vector3 position, t_vector3 rotation, t_ray *ray);

/*** Camera *******************************************************************/

struct s_camera
{
	t_vector3	position;
	t_vector3	direction;
	t_matrix44	c2w;
	void		*render;
	float		fov;
};

t_camera	*parse_camera(t_list *data, t_vector3 origin);
t_camera	*new_camera(t_vector3 position, t_vector3 direction, float fov);

/*** Logs utils ***************************************************************/

enum e_log_type
{
	INFO,
	WARN,
	ERROR,
	DEBUG,
	FATAL
};

int			log_nl(void);
int			log_msg_arg(t_log_type type, char *str, char *arg);
int			log_msg(t_log_type type, char *str);
int			is_debug_enabled(void);
int			set_debug(int debug);

/*** Scene utils **************************************************************/

struct s_options
{
	size_t	width;
	size_t	height;
	int		threads;
	size_t	chunk_width;
	size_t	chunk_height;
};

struct s_scene
{
	t_options	*render;
	t_list		*cameras;
	t_list		*lights;
	t_list		*objects;
	t_color		*ambiant;
	t_color		*background;
	int			index;
};

void		*free_scene(t_scene *scene);
t_scene		*get_scene(void);
int			load_scene(char *file);

/*** Mlx implementation *******************************************************/

struct s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
};

t_image		*mlx_init_image(t_vars *vars, t_options *params);
void		mlx_set_pixel(t_image *image, int x, int y, t_color color);
void		force_put_image(t_vars *vars, t_image *image);
void		init_window(char *file, t_scene *scene);

/*** Parsing utils ************************************************************/

int			render(t_vars *vars);
int			parse_file(t_scene *scene, char *file, int depth, t_vector3 origin);
int			col_deserialize(const char *str, t_color *color);
int			vec_deserialize(const char *str, t_vector3 *vector);
int			dir_deserialize(const char *str, t_vector3 *direction);
int			float_deserialize(char *str, float *result);
int			int_deserialize(char *str, int *result);
int			bounded_int_deserialize(char *str, int *result, int min, int max);
int			bounded_float_deserialize(char *str, float *result, float min,
				float max);
int			ulong_deserialize(char *str, unsigned long *result);
int			args_size(const char *type, int given, int expected);
int			parse_object(t_scene *sce, t_list *data, int depth, t_vector3 ori);
int			parse_node(t_list *line, t_scene *scene, int depth, t_vector3 ori);
t_options	*parse_render(t_list *data);
t_color		*parse_ambiant(t_list *data);
t_color		*parse_background(t_list *data);
t_scene		*parse(char *file);

#endif
