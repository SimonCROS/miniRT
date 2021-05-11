#include <math.h>

#include "minirt.h"
#include "renderer.h"

t_ray	compute_ray(t_options *render, t_camera *camera, float x, float y)
{
	static float	ratio;
	t_ray			ray;
	float			px;
	float			py;

	if (!ratio)
		ratio = render->width / (float) render->height;
	px = (2 * ((x + 0.5) / render->width) - 1) * camera->hlen * ratio;
	py = (1 - 2 * ((y + 0.5) / render->height)) * camera->hlen;
	ray.direction = vec3_normalize(vec3_new(px, py, -1));
	ray.direction = vec3_normalize(mat44_mul_vec(camera->c2w, ray.direction));
	ray.color = color_new(0, 0, 0);
	ray.light = 1;
	ray.origin = camera->position;
	ray.length = *get_z_buffer_value(camera->z_buffer, x, y, render->width);
	if (camera->normal_disruption)
		ray.origin.y += 0.5 * sin((double)x / (double)10);
	return (ray);
}
