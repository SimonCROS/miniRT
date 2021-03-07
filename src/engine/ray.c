#include <math.h>

#include "engine/ray.h"

t_ray	compute_ray(t_options *render, t_camera *camera, float x, float y)
{
	static float	ratio;
	static float	hypo_len;
	t_ray			ray;
	float			px;
	float			py;

	if (!ratio)
		ratio = render->width / (float) render->height;
	if (!hypo_len)
		hypo_len = tan(camera->fov / 2 * M_PI / 180);
	px = (2 * ((x + 0.5) / render->width) - 1) * hypo_len * ratio;
	py = (1 - 2 * ((y + 0.5) / render->height)) * hypo_len;
	ray.direction = vec3_normalize(vec3_new(px, py, -1));
	ray.direction = vec3_normalize(mat44_mul_vec(camera->c2w, ray.direction));
	ray.color = color_new(0, 0, 0);
	ray.light = 1;
	ray.origin = camera->position;
	ray.length = INFINITY;
	return (ray);
}
