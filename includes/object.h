#ifndef OBJECT_H
# define OBJECT_H

# include "libft.h"

typedef struct s_object	t_object;
typedef struct s_light	t_light;

typedef struct s_laser
{
	t_vector3	direction;
	float		radius;
}	t_laser;

struct s_light
{
	t_ray		(*calculate_ray)(t_light *, const t_ray *, float *);
	t_color		(*calculate_color)(t_light *, t_object *, const t_ray *,
					const t_ray *);
	float		brightness;
	t_vector3	position;
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
	t_vector3		position2;
	float			radius;
}	t_cylinder;

typedef struct s_hyperboloid
{
	t_vector3		position2;
}	t_hyperboloid;

typedef struct s_square
{
	float			width;
	t_vector3		p1;
	t_vector3		p2;
	t_vector3		p3;
	t_vector3		p4;
}	t_square;

typedef struct s_triangle
{
	t_vector3		p1;
	t_vector3		p2;
	t_vector3		p3;
	t_vector3		edge1;
	t_vector3		edge2;
}	t_triangle;

struct s_object
{
	t_bipredicate	collides;
	t_vector3		position;
	t_vector3		rotation;
	int				is_plane;
	int				is_quadric;
	t_color			color;
	t_quadric		quadric;
	union u_object_data
	{
		t_circle		circle;
		t_square		square;
		t_cylinder		cylinder;
		t_triangle		triangle;
		t_hyperboloid	hyperboloid;
	}				data;
};

t_object	*new_sphere(float diametre, t_vector3 position, t_color color);
t_object	*new_plane(t_vector3 position, t_vector3 rotation, t_color color);
t_object	*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3,
				t_color color);
t_object	*new_square(float width, t_vector3 position, t_vector3 rotation,
				t_color color);
t_object	*new_circle(float radius, t_vector3 position, t_vector3 rotation,
				t_color color);
t_object	*new_cylinder(float *attrs, t_vector3 position, t_vector3 rotation,
				t_color color);
t_object	*new_hyperboloid(float *attrs, t_vector3 p, t_color color);

t_object	*new_default_quadric(t_vector3 pos, t_vector3 rot, t_color col,
				t_bipredicate collides);
t_object	*new_default_plane(t_vector3 position, t_vector3 rotation,
				t_color color, t_bipredicate collides);
t_object	*new_default_object(t_vector3 position, t_vector3 rotation,
				t_color color, t_bipredicate collides);
t_light		*new_default_light(t_vector3 position, t_color color,
				float brightness);

t_camera	*parse_camera(t_list *data, t_vector3 origin);
t_object	*parse_plane(t_list *data, t_vector3 origin);
t_object	*parse_square(t_list *data, t_vector3 origin);
t_object	*parse_sphere(t_list *data, t_vector3 origin);
t_object	*parse_circle(t_list *data, t_vector3 origin);
t_object	*parse_cylinder(t_list *data, t_vector3 origin);
t_object	*parse_triangle(t_list *data, t_vector3 origin);
t_object	*parse_hyperboloid(t_list *data, t_vector3 origin);
t_light		*parse_light(t_list *data, t_vector3 origin);

t_light		*new_light_point(float brightness, t_vector3 position,
				t_color color);

int			collision(t_object *object, t_ray *ray);

void		project(t_vars *vars, t_object *triangle, t_scene *scene,
				t_vector3 min, t_vector3 max);

void		render_light(t_scene *sc, t_camera *cam, t_object *obj, t_ray *ray);

int			collides_quadric(t_object *object, t_ray *ray);
int			intersect_side(t_vector3 top, t_vector3 bot, t_vector3 rot,
				t_ray *ray);

#endif
