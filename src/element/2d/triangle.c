#include <stdlib.h>

#include "libft.h"

#include "element/plan.h"
#include "util/parsing.h"

t_object	*parse_triangle(t_list *data, t_vector3 origin)
{
	t_vector3	p1;
	t_vector3	p2;
	t_vector3	p3;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 5))
		return (NULL);
	if (!vec3_deserialize((char *)lst_get(data, 1), &p1)
		|| !vec3_deserialize((char *)lst_get(data, 2), &p2)
		|| !vec3_deserialize((char *)lst_get(data, 3), &p3)
		|| !color_deserialize((char *)lst_get(data, 4), &color))
		return (NULL);
	p1 = vec3_addv(p1, origin);
	p2 = vec3_addv(p2, origin);
	p3 = vec3_addv(p3, origin);
	return (new_triangle(p1, p2, p3, color));
}

int	collides_triangle(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*plan;
	t_vector3	edges[3];
	t_vector3	dists[3];

	plan = arg1;
	ray = arg2;
	edges[0] = vec3_subv(plan->data.triangle.p2, plan->data.triangle.p1);
	edges[1] = vec3_subv(plan->data.triangle.p3, plan->data.triangle.p2);
	edges[2] = vec3_subv(plan->data.triangle.p1, plan->data.triangle.p3);
	dists[0] = vec3_subv(ray->phit, plan->data.triangle.p1);
	dists[1] = vec3_subv(ray->phit, plan->data.triangle.p2);
	dists[2] = vec3_subv(ray->phit, plan->data.triangle.p3);
	if (vec3_dotv(plan->rotation, vec3_crossv(edges[0], dists[0])) > 0
		&& vec3_dotv(plan->rotation, vec3_crossv(edges[1], dists[1])) > 0
		&& vec3_dotv(plan->rotation, vec3_crossv(edges[2], dists[2])) > 0)
		return (1);
	return (0);
}

t_object	*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3, t_color col)
{
	t_object	*plan;
	t_vector3	cen;
	t_vector3	d1;
	t_vector3	d2;

	cen = vec3_divd(vec3_addv(p1, vec3_addv(p2, p3)), 3);
	d1 = vec3_subv(p2, p1);
	d2 = vec3_subv(p3, p2);
	plan = new_default_plane(cen, vec3_crossv(d1, d2), col, &collides_triangle);
	if (!plan)
		return (NULL);
	plan->data.triangle.p1 = p1;
	plan->data.triangle.p2 = p2;
	plan->data.triangle.p3 = p3;
	return (plan);
}
