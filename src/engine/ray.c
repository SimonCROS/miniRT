#include <math.h>

#include "minirt.h"

void	convertToRaster(t_options *render, t_camera *camera, t_vector3 vertexWorld)
{
	static float	ratio;
	t_vector3		vertexCamera;
	t_vector3		vertexScreen;
	t_vector3		vertexNDC;
	t_vector3		vertexRaster;

	if (!ratio)
		ratio = render->width / (float) render->height;
	vertexCamera = mat44_mul_vec(mat44_inverse(camera->c2w), vertexWorld);

	vertexScreen.x = vertexCamera.x / camera->hlen / ratio;
	vertexScreen.y = vertexCamera.y / camera->hlen;

	vertexNDC.x = (vertexScreen.x + 1) / 2;
	vertexNDC.y = (vertexScreen.y - 1) / -2;

	vertexRaster.x = vertexNDC.x * render->width - 0.5;
	vertexRaster.y = vertexNDC.y * render->height - 0.5;

	printf("%d,%d\n", (int)vertexRaster.x, (int)vertexRaster.y);
}

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
	ray.length = INFINITY;
	convertToRaster(render, camera, ray.direction);
	return (ray);
}
