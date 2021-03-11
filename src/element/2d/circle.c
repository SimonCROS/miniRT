#include <stdlib.h>
#include <math.h>

#include "convert.h"
#include "matrix.h"
#include "vector3.h"

#include "element/plan.h"

static int	rot_deserialize(const char *str, t_vector3 *vector)
{
	t_vector3	rot;

	if (!vec3_deserialize(str, &rot))
		return (0);
	if (rot.x < -1 || rot.x > 1 ||
		rot.y < -1 || rot.y > 1 ||
		rot.z < -1 || rot.z > 1)
		return (0);
	*vector = rot;
	return (1);
}

t_object	*parse_circle(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	t_vector3	rot;
	float		diametre;
	t_color		color;
	int			e;

	if (data->size != 5)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && rot_deserialize((char *)lst_get(data, 2), &rot);
	e = e && ft_atof_full((char *)lst_get(data, 3), &diametre);
	e = e && color_deserialize((char *)lst_get(data, 4), &color);
	if (!e)
		return (NULL);
	diametre = fabsf(diametre);
	return (new_circle(diametre, vec3_addv(pos, origin), rot, color));
}

int	collides_circle(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*plan;
	t_vector3	v;
	float		d2;

	plan = arg1;
	ray = arg2;
	v = vec3_subv(ray->phit, plan->position);
	d2 = vec3_length_squared(v);
	return (d2 <= plan->data.circle.radius * plan->data.circle.radius);
}

t_object	*new_circle(float diametre, t_vector3 position, t_vector3 rotation,
	t_color color)
{
	t_object	*plan;

	plan = new_default_plane(position, rotation, color, &collides_circle);
	if (!plan)
		return (NULL);
	plan->data.circle.radius = diametre / 2;
	return (plan);
}
