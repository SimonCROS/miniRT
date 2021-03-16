#ifndef PLAN_H
# define PLAN_H

# include "vector3.h"
# include "color.h"
# include "types.h"
# include "list.h"

# include "engine/ray.h"

typedef struct s_circle
{
	float			radius;
}	t_circle;

typedef struct s_sphere
{
	float			radius;
}	t_sphere;

typedef struct s_cylinder
{
	t_vector3		position2;
	float			radius;
	float			height;
	int				caps;
}	t_cylinder;

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
}	t_triangle;

typedef union u_object_data
{
	t_square		square;
	t_sphere		sphere;
	t_circle		circle;
	t_cylinder		cylinder;
	t_triangle		triangle;
}	t_object_data;

typedef struct s_object
{
	t_bipredicate	collides;
	t_vector3		position;
	t_vector3		rotation;
	int				is_plane;
	t_color			color;
	t_object_data	data;
}	t_object;

t_object	*new_sphere(float diametre, t_vector3 position, t_color color);
t_object	*new_plane(t_vector3 position, t_vector3 rotation, t_color color);
t_object	*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3,
				t_color color);
t_object	*new_square(float width, t_vector3 position, t_vector3 rotation,
				t_color color);
t_object	*new_circle(float diametre, t_vector3 position, t_vector3 rotation,
				t_color color);
t_object	*new_cylinder(float *attrs, t_vector3 position, t_vector3 rotation,
				t_color color);
t_object	*new_default_plane(t_vector3 position, t_vector3 rotation,
				t_color color, t_bipredicate collides);
t_object	*new_default_object(t_vector3 position, t_vector3 rotation,
				t_color color, t_bipredicate collides);
int			intersect_plane(t_vector3 position, t_vector3 rotation, t_ray *ray);
int			collision(t_object *object, t_ray *ray);
t_object	*parse_triangle(t_list *data, t_vector3 origin);
t_object	*parse_plane(t_list *data, t_vector3 origin);
t_object	*parse_square(t_list *data, t_vector3 origin);
t_object	*parse_sphere(t_list *data, t_vector3 origin);
t_object	*parse_circle(t_list *data, t_vector3 origin);
t_object	*parse_cylinder(t_list *data, t_vector3 origin);

#endif
