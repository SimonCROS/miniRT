#ifndef MINIRT_H
# define MINIRT_H

# include <errno.h>
# include <stdio.h>

# include "libft.h"
# include "tpool.h"
# include "graphics.h"

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
#  define K_A 0
#  define K_B 11
#  define K_C 8
#  define K_D 2
#  define K_F 3
#  define K_M 46
#  define K_N 45
#  define K_O 31
#  define K_P 35
#  define K_S 1
#  define K_U 32
#  define K_V 9
#  define K_W 13
#  define K_X 7
#  define K_Z 6
#  define K_ESP 49
#  define K_LSHIFT 257
# else
#  define K_LEFT 65361
#  define K_RIGHT 65363
#  define K_DOWN 65364
#  define K_UP 65362
#  define K_ESC 65307
#  define K_ENTER 65293
#  define K_A 113
#  define K_B 98
#  define K_C 99
#  define K_D 100
#  define K_F 102
#  define K_M 109
#  define K_N 110
#  define K_O 111
#  define K_P 112
#  define K_S 115
#  define K_U 117
#  define K_V 118
#  define K_W 122
#  define K_X 120
#  define K_Z 122
#  define K_ESP 32
#  define K_LSHIFT 65505
# endif

/*** Types ********************************************************************/

typedef float				t_noaa[1][2];
typedef float				t_ssaa4[4][2];

typedef struct s_vars		t_vars;

typedef enum e_log_type		t_log_type;

typedef struct s_scene		t_scene;
typedef struct s_options	t_options;

typedef struct s_image		t_image;
typedef enum e_click_type	t_click_type;
typedef void				(*t_pixel_writer)(void *, int, int, t_color);
typedef t_color				(*t_pixel_reader)(void *, int, int);

typedef struct s_ray		t_ray;
typedef struct s_camera		t_camera;

typedef struct s_gnl_entry	t_gnl_entry;

typedef enum e_read_status	t_read_status;

typedef enum e_impl			t_impl;

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

void		launch_game(t_vars *vars);
void		init_window(char *file, t_scene *scene);
void		force_put_image(t_vars *vars, t_image *image);
void		init_window_size(t_vars *vars, t_scene *scene);
t_image		*mlx_init_image(t_vars *vars, t_options *params);
void		mlx_set_pixel(t_image *image, int x, int y, t_color color);

void		mlx_free_image(t_image *image, t_vars *vars);

void		mlx_exit(t_vars *vars);
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
int			on_release_click(t_vars *vars, t_click_type type);
int			on_change_camera(t_vars *vars, t_camera *from, t_camera *to);

/*** Bmp implementation *******************************************************/

void		init_bmp_image(char *file, t_scene *scene);

/*** General ******************************************************************/

struct s_vars
{
	void			*mlx;
	void			*win;
	t_scene			*scene;
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
	t_vec3f	direction;
	t_vec3f	phit;
	t_vec3f	nhit;
	t_vec3f	origin;
	t_color	color;
	float	length;
	float	light;
};

int			render(t_vars *vars);
void		draw_line(t_vars *vars, t_line line, t_color color);
int			intersect_plane(t_vec3f position, t_vec3f rotation, t_ray *ray);
t_ray		compute_ray(t_options *render, t_camera *camera, float x, float y);

/*** Camera *******************************************************************/

struct s_camera
{
	t_vec3f		position;
	t_vec3f		direction;
	t_vec3f		flat;
	t_vec3f		right;
	t_vec3f		up;
	t_matrix44	c2w;
	t_matrix44	w2c;
	void		*render;
	float		*z_buffer;
	float		hlen;
	int			shadows;
	int			show_triangles;
	int			normal_disruption;
	int			sepia;
	int			sphere_up_map;
	int			color_disruption;
};

t_camera	*new_camera(t_vec3f position, t_vec3f direction, float fov);
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
int			log_prev_line(void);
int			log_msg_arg(t_log_type type, char *str, const char *arg);
int			log_msg(t_log_type type, char *str);
int			is_debug_enabled(void);
int			set_debug(int debug);

/*** Scene utils **************************************************************/

struct s_options
{
	int		width;
	int		height;
	int		threads;
	int		chunk_width;
	int		chunk_height;
	int		samples;
	float	samples_template[8][2];
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

/*** Parsing utils ************************************************************/

enum e_read_status
{
	READ_ERROR,
	READ_SUCCESS,
	READ_EOF
};

int			render(t_vars *vars);
int			parse_file(t_scene *scene, char *file, int depth, t_vec3f origin);
int			col_deserialize(const char *str, t_color *color);
int			vec_deserialize(const char *str, t_vec3f *vector);
int			dir_deserialize(const char *str, t_vec3f *direction);
int			float_deserialize(char *str, float *result);
int			int_deserialize(char *str, int *result);
int			min_int_deserialize(char *str, int *result, int min);
int			bounded_int_deserialize(char *str, int *result, int min, int max);
int			bounded_float_deserialize(char *str, float *result, float min,
				float max);
int			args_size(const char *type, int given, int expected);
int			parse_object(t_scene *sce, t_list *data, int depth, t_vec3f ori);
int			parse_node(t_list *line, t_scene *scene, int depth, t_vec3f ori);
int			read_file(char *file, int depth, t_list **nodes);
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

/*** Misc *********************************************************************/

enum e_impl
{
	USE_MLX,
	USE_BMP
};

void		get_samples_template(int aa, void *buf);
t_color		to_sepia(t_color in);

#endif
