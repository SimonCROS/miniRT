#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_circle(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	t_vector3	rot;
	float		diametre;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 5))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &diametre)
		|| !col_deserialize((char *)lst_get(data, 4), &color))
		return (NULL);
	diametre = fabsf(diametre);
	return (new_circle(diametre, vec3_addv(pos, origin), rot, color));
}

int	collides_circle(t_object *plan, t_ray *ray)
{
	t_vector3	v;
	float		d2;

	v = vec3_subv(ray->phit, plan->position);
	d2 = vec3_length_squared(v);
	return (d2 <= plan->data.circle.radius * plan->data.circle.radius);
}

t_object	*new_circle(float diametre, t_vector3 position, t_vector3 rotation,
	t_color color)
{
	t_object	*plan;

	plan = new_default_plane(position, rotation, color,
			(t_bipre)collides_circle);
	if (!plan)
		return (NULL);
	plan->data.circle.radius = diametre / 2;
	return (plan);
}
