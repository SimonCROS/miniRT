#include "minirt.h"
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
	pixel.x = box.min.x;
	while (pixel.x <= box.max.x - 1)
	{
		pixel.y = box.min.y;
		while (pixel.y <= box.max.y - 1)
		{
			buf_z = get_z_buffer_value(camera->z_buffer, pixel.x, pixel.y,
					scene->render->width);
			ray = compute_ray(scene->render, camera, pixel.x, pixel.y);
			if (collides_triangle(triangle, &ray) && ray.length < *buf_z)
			{
				*buf_z = ray.length;
				render_light(scene, camera, triangle, &ray);
				vars->set_pixel(camera->render, pixel.x, pixel.y, ray.color);
			}
			pixel.y++;
		}
		pixel.x++;
	}
}

void	project(t_vars *vars, t_object *obj, t_bounding_box chunk)
{
	t_vec2f			s0;
	t_vec2f			s1;
	t_vec2f			s2;
	t_scene			*scene;
	t_camera		*cam;
	int				behind;
	t_bounding_box	box;

	scene = vars->scene;
	cam = vars->camera;
	behind = convert_to_raster(&s0, scene->render, cam, obj->data.triangle.p1);
	behind += convert_to_raster(&s1, scene->render, cam, obj->data.triangle.p2);
	behind += convert_to_raster(&s2, scene->render, cam, obj->data.triangle.p3);
	if (!behind && cam->show_triangles)
	{	
		if (!bounding_box_intersects(box, chunk))
			return ;
		draw_line(vars, s0.x, s0.y, s1.x, s1.y, color_new(224, 211, 25));
		draw_line(vars, s1.x, s1.y, s2.x, s2.y, color_new(224, 211, 25));
		draw_line(vars, s2.x, s2.y, s0.x, s0.y, color_new(224, 211, 25));
	}
	else if (behind != 3)
	{
		if (cam->shadows && behind)
			box = chunk;
		else
			box = bounding_box_intersection(bounding_box_fromf3(s0, s1, s2),
					chunk);
		ray_trace_clip(vars, obj, box);
	}
}
