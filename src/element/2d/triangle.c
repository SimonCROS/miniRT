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

int	pre_collision(t_object *triangle, float x, float y)
{
	if (x < triangle->data.triangle.min_raster.x
		|| x > triangle->data.triangle.max_raster.x
		|| y < triangle->data.triangle.min_raster.y
		|| y > triangle->data.triangle.max_raster.y)
		return (FALSE);
	return (TRUE);
}

static t_vector3	convert_to_raster(t_options *render, t_camera *camera,
	t_vector3 vertexWorld)
{
	t_vector3		vertexView;
	t_vector3		vertexCamera;
	t_vector3		vertexScreen;
	t_vector3		vertexNDC;
	t_vector3		vertexRaster;

	vertexView = vec3_normalize(vec3_subv(camera->position, vertexWorld));
	vertexCamera = mat44_mul_vec(mat44_inverse(camera->c2w), vertexView);
	vertexScreen.x = -vertexCamera.x / camera->hlen
		/ (render->width / (float) render->height) / vertexCamera.z;
	vertexScreen.y = -vertexCamera.y / camera->hlen / vertexCamera.z;
	vertexNDC.x = (vertexScreen.x + 1) / 2;
	vertexNDC.y = (vertexScreen.y - 1) / -2;
	vertexRaster.x = vertexNDC.x * render->width - 0.5;
	vertexRaster.y = vertexNDC.y * render->height - 0.5;
	return (vertexRaster);
}

void	load_bounds(t_object *triangle, t_camera *camera, t_options *options)
{
	t_vector3	s0;
	t_vector3	s1;
	t_vector3	s2;

	s0 = convert_to_raster(render, camera, triangle->data.triangle.p1);
	s1 = convert_to_raster(render, camera, triangle->data.triangle.p2);
	s2 = convert_to_raster(render, camera, triangle->data.triangle.p3);
	triangle->data.triangle.min_raster.x
		= floorf(fminf(s0.x, fminf(s1.x, s2.x)));
	triangle->data.triangle.min_raster.y
		= floorf(fminf(s0.y, fminf(s1.y, s2.y)));
	triangle->data.triangle.max_raster.x
		= floorf(fmaxf(s0.x, fmaxf(s1.x, s2.x)));
	triangle->data.triangle.max_raster.y
		= floorf(fmaxf(s0.y, fmaxf(s1.y, s2.y)));
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
	t_object	*triangle;
	t_vector3	edge1;
	t_vector3	edge2;

	edge1 = vec3_subv(p2, p1);
	edge2 = vec3_subv(p3, p1);
	triangle = new_default_object(p1, vec3_crossv(edge1, edge2), col,
			(t_bipre)collides_triangle);
	if (!triangle)
		return (NULL);
	triangle->data.triangle.p1 = p1;
	triangle->data.triangle.p2 = p2;
	triangle->data.triangle.p3 = p3;
	triangle->data.triangle.edge1 = edge1;
	triangle->data.triangle.edge2 = edge2;
	triangle->pre_collision = pre_collision;
	return (triangle);
}
