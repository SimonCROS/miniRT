#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_square(t_list *data, t_vec3f origin)
{
	t_vec3f	pos;
	t_vec3f	rot;
	float	width;
	t_color	color;

	if (!args_size(lst_first(data), data->size, 5))
		return (NULL);
	if (!vec3_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &width)
		|| !color_deserialize((char *)lst_get(data, 4), &color))
		return (NULL);
	width = fabsf(width);
	return (new_square(width, vec3_add(pos, origin), rot, color));
}

static int	orient(t_vec3f a, t_vec3f b, t_vec3f c, t_vec3f n)
{
	int	result;

	result = vec3_dot(vec3_cross(vec3_sub(b, a), vec3_sub(c, a)), n);
	if (result < 0)
		return (-1);
	return (result > 0);
}

int	collides_square(t_object *plan, t_ray *ray)
{
	t_square	*square;
	int			in;

	square = &(plan->data.square);
	in = orient(ray->phit, square->p1, square->p2, plan->rotation);
	in += orient(ray->phit, square->p2, square->p3, plan->rotation);
	in += orient(ray->phit, square->p3, square->p4, plan->rotation);
	in += orient(ray->phit, square->p4, square->p1, plan->rotation);
	if (in == 4 || in == -4)
		return (1);
	return (0);
}

t_object	*new_square(float width, t_vec3f position, t_vec3f rotation,
	t_color color)
{
	t_object	*plan;
	float		mid;
	t_vec3f		diagonal;

	plan = new_default_plane(position, rotation, color,
			(t_bipre)collides_square);
	if (!plan)
		return (NULL);
	mid = (width * sqrtf(2)) * 0.5;
	diagonal = vec3_cross(rotation, vec3_new(1, 0, 0));
	if (vec3_is_null(diagonal))
		diagonal = vec3_new(0, 1, 0);
	diagonal = vec3_rotate_axis(diagonal, rotation, M_PI_4);
	diagonal = vec3_muld(vec3_normalize(diagonal), mid);
	plan->data.square.p1 = vec3_add(diagonal, position);
	plan->data.square.p3 = vec3_add(vec3_negate(diagonal), position);
	diagonal = vec3_rotate_axis(diagonal, rotation, M_PI_2);
	diagonal = vec3_muld(vec3_normalize(diagonal), mid);
	plan->data.square.p2 = vec3_add(diagonal, position);
	plan->data.square.p4 = vec3_add(vec3_negate(diagonal), position);
	plan->data.square.width = width;
	return (plan);
}
