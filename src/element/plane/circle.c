#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_circle(t_list *data, t_vec3f origin)
{
	t_vec3f	pos;
	t_vec3f	rot;
	float	diametre;
	t_color	color;

	if (!args_size(lst_first(data), data->size, 5))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &diametre)
		|| !col_deserialize((char *)lst_get(data, 4), &color))
		return (NULL);
	diametre = fabsf(diametre);
	return (new_circle(diametre, vec3_add(pos, origin), rot, color));
}

static int	collides_circle(t_object *plan, t_ray *ray)
{
	return (vec3_distance_squared(ray->phit, plan->position)
		<= plan->data.circle.radius * plan->data.circle.radius);
}

t_object	*new_circle(float diametre, t_vec3f position, t_vec3f rotation,
	t_color color)
{
	t_object	*plan;

	plan = new_default_plane(position, rotation, color,
			(t_bipre)collides_circle);
	if (!plan)
		return (NULL);
	plan->data.circle.radius = diametre * 0.5;
	return (plan);
}
