#include <stdlib.h>
#include <math.h>

#include "convert.h"
#include "matrix.h"
#include "vector3.h"

#include "element/plan.h"

int	parse_circle(t_list *data)
{
	(void)data;
	return (0);
}

int	collides_circle(void *a1, void *a2)
{
	t_ray		*ray;
	t_object	*plan;
	t_vector3	v;
	float		d2;

	plan = a1;
	ray = a2;
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
