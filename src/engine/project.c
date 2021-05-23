#include <math.h>
#include "renderer.h"
#include "object.h"

static int	convert_to_raster(t_vec2f *vertexRaster, t_options *render,
	t_camera *camera, t_vec3f vertexWorld)
{
	t_vec3f	vertexView;
	t_vec3f	vertexCamera;
	t_vec2f	vertexScreen;
	t_vec2f	vertexNDC;
	int		behind;

	vertexView = vec3_normalize(vec3_sub(camera->position, vertexWorld));
	vertexCamera = mat44_mul_vec(camera->w2c, vertexView);
	behind = vertexCamera.z < 0;
	vertexCamera.z = fabsf(vertexCamera.z);
	vertexScreen.x = -vertexCamera.x / camera->hlen / (render->width
			/ (float)render->height) / vertexCamera.z;
	vertexScreen.y = -vertexCamera.y / camera->hlen / vertexCamera.z;
	vertexNDC.x = (vertexScreen.x + 1) * 0.5;
	vertexNDC.y = (vertexScreen.y - 1) * -0.5;
	vertexRaster->x = vertexNDC.x * render->width - 0.5;
	vertexRaster->y = vertexNDC.y * render->height - 0.5;
	return (behind);
}

static void	ray_trace_clip(t_vars *vars, t_object *triangle, t_bounding_box box)
{
	t_vec2i		pixel;
	t_ray		ray;
	float		*buf_z;
	t_scene		*scene;
	t_camera	*camera;

	scene = vars->scene;
	camera = vars->camera;
	pixel.x = box.min.x - 1;
	while (++pixel.x <= box.max.x - 1)
	{
		pixel.y = box.min.y - 1;
		while (++pixel.y <= box.max.y - 1)
		{
			buf_z = get_z_buffer_value(camera->z_buffer, pixel.x, pixel.y,
					scene->render->width);
			ray = compute_ray(scene->render, camera, pixel.x, pixel.y);
			if (triangle->collides(triangle, &ray) && ray.length < *buf_z)
			{
				*buf_z = ray.length;
				render_light(scene, camera, triangle, &ray);
				vars->set_pixel(camera->render, pixel.x, pixel.y, ray.color);
			}
		}
	}
}

static int	get_points(t_vec2f *points, t_vars *vars, t_object *object)
{
	int	behind;

	behind = convert_to_raster(points + 0, vars->scene->render, vars->camera,
			object->data.triangle.p1);
	behind += convert_to_raster(points + 1, vars->scene->render, vars->camera,
			object->data.triangle.p2);
	behind += convert_to_raster(points + 2, vars->scene->render, vars->camera,
			object->data.triangle.p3);
	return (behind);
}

void	project(t_vars *vars, t_object *object, t_bounding_box chunk)
{
	t_vec2f			p[3];
	t_camera		*cam;
	t_bounding_box	box;
	int				behind;

	cam = vars->camera;
	behind = get_points(p, vars, object);
	box = bounding_box_fromf3(p[0], p[1], p[2]);
	if (cam->show_triangles)
	{
		if (behind || !bounding_box_intersects(box, chunk))
			return ;
		draw_line(vars, line_fromf(p[0], p[1]), color_new(224, 211, 25));
		draw_line(vars, line_fromf(p[1], p[2]), color_new(224, 211, 25));
		draw_line(vars, line_fromf(p[2], p[0]), color_new(224, 211, 25));
	}
	else if (behind != 3)
	{
		if (cam->shadows && behind)
			box = chunk;
		else
			box = bounding_box_intersection(box, chunk);
		ray_trace_clip(vars, object, box);
	}
}
