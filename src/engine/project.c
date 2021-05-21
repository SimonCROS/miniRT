#include "minirt.h"
#include "object.h"

static t_vec3f	convert_to_raster(t_options *render, t_camera *camera,
	t_vec3f vertexWorld)
{
	t_vec3f	vertexView;
	t_vec3f	vertexCamera;
	t_vec3f	vertexScreen;
	t_vec3f	vertexNDC;
	t_vec3f	vertexRaster;

	vertexView = vec3_normalize(vec3_sub(camera->position, vertexWorld));
	vertexCamera = mat44_mul_vec(camera->w2c, vertexView);
	vertexRaster.z = vertexCamera.z;
	vertexCamera.z = fabsf(vertexCamera.z);
	vertexScreen.x = -vertexCamera.x / camera->hlen / (render->width
			/ (float)render->height) / vertexCamera.z;
	vertexScreen.y = -vertexCamera.y / camera->hlen / vertexCamera.z;
	vertexNDC.x = (vertexScreen.x + 1) * 0.5;
	vertexNDC.y = (vertexScreen.y - 1) * -0.5;
	vertexRaster.x = vertexNDC.x * render->width - 0.5;
	vertexRaster.y = vertexNDC.y * render->height - 0.5;
	return (vertexRaster);
}

static void	ray_trace_clip(t_vars *vars, t_object *triangle, t_vec3f min, t_vec3f max)
{
	size_t		x;
	size_t		y;
	t_ray		ray;
	float		*buf_z;
	t_vec3f	min_raster;
	t_vec3f	max_raster;

	min_raster.x = fminf3(s0.x, s1.x, s2.x);
	min_raster.y = fminf3(s0.y, s1.y, s2.y);
	max_raster.x = ceilf(fmaxf3(s0.x, s1.x, s2.x));
	max_raster.y = ceilf(fmaxf3(s0.y, s1.y, s2.y));
	min_raster.x = fmaxf(min_raster.x, min.x);
	min_raster.y = fmaxf(min_raster.y, min.y);
	max_raster.x = fminf(max_raster.x, max.x);
	max_raster.y = fminf(max_raster.y, max.y);
	if ((s0.z < 0 || s1.z < 0 || s2.z < 0) && camera->shadows)
	{
		min_raster = min;
		max_raster = max;
	}
	x = min_raster.x;
	while (x <= max_raster.x - 1)
	{
		y = min_raster.y;
		while (y <= max_raster.y - 1)
		{
			buf_z = get_z_buffer_value(camera->z_buffer, x, y, scene->render->width);
			ray = compute_ray(scene->render, camera, x, y);
			if (collides_triangle(triangle, &ray) && ray.length < *buf_z)
			{
				*buf_z = ray.length;
				render_light(scene, camera, triangle, &ray);
				vars->set_pixel(camera->render, x, y, ray.color);
			}
			y++;
		}
		x++;
	}
}

void	project(t_vars *vars, t_object *triangle, t_vec3f min, t_vec3f max)
{
	t_vec3f	s0;
	t_vec3f	s1;
	t_vec3f	s2;
	t_scene		*scene;
	t_camera	*camera;

	scene = vars->scene;
	camera = vars->camera;
	s0 = convert_to_raster(scene->render, camera, triangle->data.triangle.p1);
	s1 = convert_to_raster(scene->render, camera, triangle->data.triangle.p2);
	s2 = convert_to_raster(scene->render, camera, triangle->data.triangle.p3);
	if (s0.z < 0 && s1.z < 0 && s2.z < 0)
		return ;
	if (camera->show_triangles)
	{	
		if ((s0.x < 0 && s1.x < 0 && s2.x < 0)
			|| (s0.x > max.x && s1.x > max.x && s2.x > max.x))
			return ;
		if ((s0.y < 0 && s1.y < 0 && s2.y < 0)
			|| (s0.y > max.y && s1.y > max.y && s2.y > max.y))
			return ;
		draw_line(vars, s0.x, s0.y, s1.x, s1.y, color_new(224, 211, 25));
		draw_line(vars, s1.x, s1.y, s2.x, s2.y, color_new(224, 211, 25));
		draw_line(vars, s2.x, s2.y, s0.x, s0.y, color_new(224, 211, 25));
	}
	else
		ray_trace_clip()
}