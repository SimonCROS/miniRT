#include "minirt.h"
#include "object.h"
#include "renderer.h"

#include <math.h>

t_object	*parse_triangle(t_list *data, t_vec3f origin)
{
	t_vec3f	p1;
	t_vec3f	p2;
	t_vec3f	p3;
	t_color	color;

	if (!args_size(lst_first(data), data->size, 5))
		return (NULL);
	if (!vec3_deserialize((char *)lst_get(data, 1), &p1)
		|| !vec3_deserialize((char *)lst_get(data, 2), &p2)
		|| !vec3_deserialize((char *)lst_get(data, 3), &p3)
		|| !color_deserialize((char *)lst_get(data, 4), &color))
		return (NULL);
	p1 = vec3_add(p1, origin);
	p2 = vec3_add(p2, origin);
	p3 = vec3_add(p3, origin);
	return (new_triangle(p1, p2, p3, color));
}

static int	collides_triangle_2(t_object *object, t_ray *ray, t_vec3f tvec,
	float p[2])
{
	t_vec3f	qvec;
	float	invdet;
	float	u;
	float	v;
	float	t;

	invdet = p[0];
	u = p[1];
	qvec = vec3_cross(tvec, object->data.triangle.edge1);
	v = invdet * vec3_dot(ray->direction, qvec);
	if (v < 0 || u + v > 1)
		return (FALSE);
	t = invdet * vec3_dot(object->data.triangle.edge2, qvec);
	if (t > __FLT_EPSILON__)
	{
		ray->length = t;
		ray->phit = vec3_add(vec3_muld(ray->direction, t), ray->origin);
		ray->nhit = object->rotation;
		if (vec3_dot(object->rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		return (TRUE);
	}
	return (FALSE);
}

/**
 * @brief Triangle intersection with ray using Möller–Trumbore algorithm
 */
static int	collides_triangle(t_object *object, t_ray *ray)
{
	t_vec3f	pvec;
	t_vec3f	tvec;
	float	u;
	float	det;
	float	invdet;

	pvec = vec3_cross(ray->direction, object->data.triangle.edge2);
	det = vec3_dot(object->data.triangle.edge1, pvec);
	if (det > -__FLT_EPSILON__ && det < __FLT_EPSILON__)
		return (FALSE);
	invdet = 1 / det;
	tvec = vec3_sub(ray->origin, object->data.triangle.p1);
	u = invdet * vec3_dot(tvec, pvec);
	if (u < 0 || u > 1)
		return (FALSE);
	return (collides_triangle_2(object, ray, tvec, (float [2]){invdet, u}));
}

t_object	*new_triangle(t_vec3f p1, t_vec3f p2, t_vec3f p3, t_color col)
{
	t_object	*triangle;
	t_vec3f		edge1;
	t_vec3f		edge2;

	edge1 = vec3_sub(p2, p1);
	edge2 = vec3_sub(p3, p1);
	triangle = new_default_object(p1, vec3_cross(edge1, edge2), col,
			(t_bipre)collides_triangle);
	if (!triangle)
		return (NULL);
	triangle->data.triangle.p1 = p1;
	triangle->data.triangle.p2 = p2;
	triangle->data.triangle.p3 = p3;
	triangle->data.triangle.edge1 = edge1;
	triangle->data.triangle.edge2 = edge2;
	return (triangle);
}
