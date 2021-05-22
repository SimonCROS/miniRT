#ifndef OBJECT_H
# define OBJECT_H

# include "libft.h"

typedef struct s_object	t_object;
typedef struct s_light	t_light;

typedef t_color			(*t_calc_color)(const t_camera *cam, const t_ray *ray);

typedef struct s_laser
{
	t_vec3f	direction;
	float		radius;
}	t_laser;

struct s_light
{
	t_ray		(*calculate_ray)(t_light *, const t_ray *, float *);
	t_color		(*calculate_color)(t_light *, t_object *, const t_ray *, float);
	float		brightness;
	t_vec3f	position;
	t_color		color;
	union u_light_data
	{
		t_laser	laser;
	}			data;
};

typedef struct s_circle
{
	float			radius;
}	t_circle;

typedef struct s_cylinder
{
	t_vec3f		position2;
	float			radius;
}	t_cylinder;

typedef struct s_compound
{
	t_list			*elements;
}	t_compound;

typedef struct s_hyperboloid
{
	t_vec3f		position2;
}	t_hyperboloid;

typedef struct s_square
{
	float			width;
	t_vec3f		p1;
	t_vec3f		p2;
	t_vec3f		p3;
	t_vec3f		p4;
}	t_square;

typedef struct s_triangle
{
	t_vec3f		p1;
	t_vec3f		p2;
	t_vec3f		p3;
	t_vec3f		edge1;
	t_vec3f		edge2;
}	t_triangle;

struct s_object
{
	t_bipredicate	collides;
	t_calc_color	post_light_calculation;
	t_vec3f		position;
	t_vec3f		rotation;
	int				is_plane;
	int				is_quadric;
	t_color			color;
	t_quadric		quadric;
	union u_object_data
	{
		t_circle		circle;
		t_square		square;
		t_compound		compound;
		t_cylinder		cylinder;
		t_triangle		triangle;
		t_hyperboloid	hyperboloid;
	}				data;
};

t_object	*new_sphere(float diametre, t_vec3f position, t_color color);
t_object	*new_plane(t_vec3f position, t_vec3f rotation, t_color color);
t_object	*new_triangle(t_vec3f p1, t_vec3f p2, t_vec3f p3,
				t_color color);
t_object	*new_square(float width, t_vec3f position, t_vec3f rotation,
				t_color color);
t_object	*new_circle(float radius, t_vec3f position, t_vec3f rotation,
				t_color color);
t_object	*new_cylinder(float attrs[2], t_vec3f position,
				t_vec3f rotation, t_color color);
t_object	*new_hyperboloid(float attrs[5], t_vec3f p, t_color color);

t_object	*new_default_quadric(t_vec3f pos, t_vec3f rot, t_color col,
				t_bipredicate collides);
t_object	*new_default_plane(t_vec3f position, t_vec3f rotation,
				t_color color, t_bipredicate collides);
t_object	*new_default_object(t_vec3f position, t_vec3f rotation,
				t_color color, t_bipredicate collides);
t_light		*new_default_light(t_vec3f position, t_color color,
				float brightness);
t_object	*new_default_compound_object(t_vec3f position, t_vec3f rotation,
				t_color color);

t_camera	*parse_camera(t_list *data, t_vec3f origin);
int			parse_cube(t_list *data, t_vec3f origin, t_list *objects);
int			parse_pyramid(t_list *data, t_vec3f origin, t_list *objects);
t_object	*parse_plane(t_list *data, t_vec3f origin);
t_object	*parse_square(t_list *data, t_vec3f origin);
t_object	*parse_sphere(t_list *data, t_vec3f origin);
t_object	*parse_circle(t_list *data, t_vec3f origin);
t_object	*parse_cylinder(t_list *data, t_vec3f origin);
t_object	*parse_triangle(t_list *data, t_vec3f origin);
t_object	*parse_hyperboloid(t_list *data, t_vec3f origin);
t_light		*parse_light_point(t_list *data, t_vec3f origin);
t_light		*parse_light_laser(t_list *data, t_vec3f origin);

t_light		*new_light_point(float brightness, t_vec3f position,
				t_color color);
t_light		*new_light_laser(float s[2], t_vec3f position,
				t_vec3f direction, t_color color);

int			collision(t_object *object, t_ray *ray);

void		project(t_vars *vars, t_object *obj, t_bounding_box chunk);

void		render_light(t_scene *sc, t_camera *cam, t_object *obj, t_ray *ray);

int			collides_quadric(t_object *object, t_ray *ray);
int			intersect_side(t_vec3f top, t_vec3f bot, t_vec3f rot,
				t_ray *ray);

t_object	*nearest_object(t_list *objects, t_ray *ray);

#endif
