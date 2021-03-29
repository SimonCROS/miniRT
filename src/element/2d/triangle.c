#include "minirt.h"
#include "object.h"

#include <math.h>

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

static int	collides_triangle(t_object *object, t_ray *ray)
{
	t_vector3 pvec = (t_vector3) { 0,0,0 };
	t_vector3 tvec = (t_vector3) { 0,0,0 };
	t_vector3 qvec = (t_vector3) { 0,0,0 };
	float det, invdet, u, v;

	pvec = vec3_crossv(ray->direction, object->data.triangle.edge2);
	det = vec3_dotv(object->data.triangle.edge1, pvec);
	if (det > -__FLT_EPSILON__ && det < __FLT_EPSILON__)
		return (FALSE);
	invdet = 1 / det;
	tvec = vec3_subv(ray->origin, object->data.triangle.p1);
	u = invdet * vec3_dotv(tvec, pvec);
	if (u < 0 || u > 1)
		return (FALSE);
	qvec = vec3_crossv(tvec, object->data.triangle.edge1);
	v = invdet * vec3_dotv(ray->direction, qvec);
	if (v < 0 || u + v > 1)
		return (FALSE);
	float t = invdet * vec3_dotv(object->data.triangle.edge2, qvec);
	if (t > __FLT_EPSILON__)
	{
		ray->length = t;
		ray->phit = vec3_addv(vec3_muld(ray->direction, t), ray->origin);
		ray->nhit = object->rotation;
		if (vec3_dotv(object->rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		return (TRUE);
	}
	return (FALSE);
}

t_object	*new_triangle(t_vector3 p1, t_vector3 p2, t_vector3 p3, t_color col)
{
	t_object	*plan;
	t_vector3	edge1;
	t_vector3	edge2;

	edge1 = vec3_subv(p2, p1);
	edge2 = vec3_subv(p3, p1);
	plan = new_default_object(p1, vec3_crossv(edge1, edge2), col,
			(t_bipre)collides_triangle);
	if (!plan)
		return (NULL);
	plan->data.triangle.p1 = p1;
	plan->data.triangle.p2 = p2;
	plan->data.triangle.p3 = p3;
	plan->data.triangle.edge1 = edge1;
	plan->data.triangle.edge2 = edge2;
	return (plan);
}
