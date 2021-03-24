#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_square(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	t_vector3	rot;
	float		width;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 5))
		return (NULL);
	if (!vec3_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &width)
		|| !color_deserialize((char *)lst_get(data, 4), &color))
		return (NULL);
	width = fabsf(width);
	return (new_square(width, vec3_addv(pos, origin), rot, color));
}

static int	orient(t_vector3 a, t_vector3 b, t_vector3 c, t_vector3 n)
{
	int	result;

	result = vec3_dotv(vec3_crossv(vec3_subv(b, a), vec3_subv(c, a)), n);
	if (result > 0)
		return (1);
	if (result < 0)
		return (-1);
	return (0);
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

t_object	*new_square(float width, t_vector3 position, t_vector3 rotation,
	t_color color)
{
	t_object	*plan;
	float		mid;
	t_vector3	diagonal;

	plan = new_default_plane(position, rotation, color,
			(t_bipre)collides_square);
	if (!plan)
		return (NULL);
	mid = (width * sqrtf(2)) / 2;
	diagonal = vec3_crossv(rotation, vec3_new(1, 0, 0));
	if (vec3_length(diagonal) == 0)
		diagonal = vec3_new(0, 1, 0);
	diagonal = vec3_rotate_axis(diagonal, rotation, M_PI_4);
	diagonal = vec3_muld(vec3_normalize(diagonal), mid);
	plan->data.square.p1 = vec3_addv(diagonal, position);
	plan->data.square.p3 = vec3_addv(vec3_negate(diagonal), position);
	diagonal = vec3_rotate_axis(diagonal, rotation, M_PI_2);
	diagonal = vec3_muld(vec3_normalize(diagonal), mid);
	plan->data.square.p2 = vec3_addv(diagonal, position);
	plan->data.square.p4 = vec3_addv(vec3_negate(diagonal), position);
	plan->data.square.width = width;
	return (plan);
}
