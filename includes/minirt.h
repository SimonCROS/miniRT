#ifndef MINIRT_H
# define MINIRT_H

# include <errno.h>
# include <stdio.h>

# include "libft.h"
# include "tpool.h"

/*** Definitions **************************************************************/

# define MAX_THREADS 16

# define BUFF_SIZE 50

# ifdef __APPLE__
#  define K_LEFT 123
#  define K_RIGHT 124
#  define K_DOWN 125
#  define K_UP 126
#  define K_ESC 53
#  define K_ENTER 36
#  define K_W 13
#  define K_S 1
#  define K_A 0
#  define K_D 2
#  define K_F 3
#  define K_O 31
#  define K_P 35
#  define K_M 46
#  define K_N 45
#  define K_ESP 49
#  define K_LSHIFT 257
# else
#  define K_LEFT 65361
#  define K_RIGHT 65363
#  define K_DOWN 65364
#  define K_UP 65362
#  define K_ESC 65307
#  define K_ENTER 65293
#  define K_W 122
#  define K_S 115
#  define K_A 113
#  define K_D 100
#  define K_O 111
#  define K_P 112
#  define K_F 102
#  define K_M 109
#  define K_N 110
#  define K_ESP 32
#  define K_LSHIFT 65505
# endif

/*** Types ********************************************************************/

typedef struct s_vars		t_vars;

typedef enum e_log_type		t_log_type;

typedef struct s_scene		t_scene;
typedef struct s_options	t_options;

typedef struct s_image		t_image;
typedef enum e_click_type	t_click_type;
typedef void				(*t_pixel_writer)(void *, int, int, t_color);

typedef struct s_ray		t_ray;
typedef struct s_camera		t_camera;

typedef struct s_gnl_entry	t_gnl_entry;

/*** Mlx implementation *******************************************************/

struct s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
};

# ifdef __APPLE__

#  define SCROLL_UP 5
#  define SCROLL_DOWN 4

enum e_click_type
{
	CLICK_NONE,
	CLICK_LEFT,
	CLICK_RIGHT,
	CLICK_MIDDLE
};
# else

#  define SCROLL_UP 4
#  define SCROLL_DOWN 5

enum e_click_type
{
	CLICK_NONE,
	CLICK_LEFT,
	CLICK_MIDDLE,
	CLICK_RIGHT
};
# endif

t_image		*mlx_init_image(t_vars *vars, t_options *params);
void		mlx_set_pixel(t_image *image, int x, int y, t_color color);
void		force_put_image(t_vars *vars, t_image *image);
void		init_window(char *file, t_scene *scene);

void		mlx_free_image(t_image *image, t_vars *vars);

void		reset_keys(t_vars *vars);

/*== Hooks ==*/

int			key_pressed_hook(int key, t_vars *vars);
int			key_released_hook(int key, t_vars *vars);
int			mouse_pressed_hook(int button, int x, int y, t_vars *vars);
int			mouse_released_hook(int button, int x, int y, t_vars *vars);
int			mouse_moved_hook(int x, int y, t_vars *vars);
int			close_hook(t_vars *vars);

/*== Events ==*/

int			on_close(t_vars *vars);
int			on_scroll(t_vars *vars, int direction);
int			on_drag(t_vars *vars, t_click_type type);
int			on_click(t_vars *vars, t_click_type type);
int			on_change_camera(t_vars *vars);
int			on_release_click(t_vars *vars, t_click_type type);

/*** Bmp implementation *******************************************************/

void		init_bmp_image(char *file, t_scene *scene);

/*** General ******************************************************************/

struct s_vars
{
	void			*mlx;
	void			*win;
	t_camera		*camera;
	t_bifunction	init_image;
	t_pixel_writer	set_pixel;
	t_biconsumer	on_finished;
	t_biconsumer	free_image;
	t_consumer		on_exit;
	int				flush;
	int				forward;
	int				backward;
	int				left;
	int				right;
	int				up;
	int				down;
	int				scroll_direction;
	int				mouse_x;
	int				mouse_y;
	int				mouse_x_from;
	int				mouse_y_from;
	int				click_type;
	int				cam_left;
	int				cam_right;
	int				cam_up;
	int				cam_down;
};

void		exit_minirt(t_vars *vars, t_tpool *pool, void *other, int __status);

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

int			render(t_vars *vars);
void		draw_line(t_vars *vars, int x0, int y0, int x1, int y1, t_color co);
t_ray		compute_ray(t_options *render, t_camera *camera, float x, float y);
int			intersect_plane(t_vector3 position, t_vector3 rotation, t_ray *ray);

/*** Camera *******************************************************************/

struct s_camera
{
	t_vector3	position;
	t_vector3	direction;
	t_vector3	flat;
	t_vector3	right;
	t_vector3	up;
	t_matrix44	c2w;
	t_matrix44	w2c;
	void		*render;
	float		*z_buffer;
	float		hlen;
	int			shadows;
	int			show_triangles;
};

t_camera	*new_camera(t_vector3 position, t_vector3 direction, float fov);
void		reload_camera(t_camera *camera);

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
int			log_cr(void);
int			log_msg_arg(t_log_type type, char *str, const char *arg);
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
	t_list		*triangles;
	t_color		*ambiant;
	t_color		*background;
	int			index;
};

void		*free_scene(t_scene *scene);
t_scene		*get_scene(void);
int			load_scene(char *file);

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

/*** GNL **********************************************************************/

struct s_gnl_entry
{
	void	*next;
	int		fd;
	char	*content;
};

char		**ft_split_first(char *s, char c);
int			get_next_line(int fd, char **line);
int			gnl_init(char ***current, char **tmp_line, ssize_t *result);

#endif
