#include <math.h>

#include "renderer.h"
#include "object.h"

// static int	in_light(t_scene *scene, t_object *object, float light_dist2,
// 	t_ray *ray)
// {
// 	t_iterator	obj_iterator;
// 	t_object	*obj_test;

// 	obj_iterator = iterator_new(scene->objects);
// 	while (iterator_has_next(&obj_iterator))
// 	{
// 		obj_test = iterator_next(&obj_iterator);
// 		if (obj_test != object && collision(obj_test, ray)
// 			&& ray->length * ray->length <= light_dist2)
// 			return (FALSE);
// 	}
// 	return (TRUE);
// }

// static void	render_light(t_scene *scene, t_object *object, t_ray *ray)
// {
// 	t_light		*light;
// 	t_iterator	lightIterator;
// 	t_ray		light_ray;
// 	t_vector3	lightDir;
// 	float		light_distance2;

// 	ray->color = color_mul(object->color, *(scene->ambiant));
// 	lightIterator = iterator_new(scene->lights);
// 	while (iterator_has_next(&lightIterator))
// 	{
// 		light = iterator_next(&lightIterator);
// 		lightDir = vec3_subv(light->position, ray->phit);
// 		light_distance2 = vec3_length_squared(lightDir);
// 		lightDir = vec3_normalize(lightDir);
// 		light_ray.origin = ray->phit;
// 		light_ray.direction = lightDir;
// 		light_ray.length = INFINITY;
// 		ray->color = color_add(ray->color, color_mul(object->color,
// 					color_mulf(color_mulf(light->color, light->brightness),
// 						in_light(scene, object, light_distance2, &light_ray)
// 						* light->brightness
// 						* fmaxf(0, vec3_dotv(lightDir, ray->nhit)))));
// 	}
// }

t_vector3	convertToRaster(t_options *render, t_camera *camera, t_vector3 vertexWorld)
{
	static float	ratio;
	t_vector3		direction;
	t_vector3		vertexCamera;
	t_vector3		vertexScreen;
	t_vector3		vertexNDC;
	t_vector3		vertexRaster;

	if (!ratio)
		ratio = render->width / (float) render->height;
	direction = vec3_normalize(vec3_subv(camera->position, vertexWorld));
	vertexCamera = mat44_mul_vec(mat44_inverse(camera->c2w), direction);
	vertexScreen.x = -vertexCamera.x / camera->hlen / ratio / vertexCamera.z;
	vertexScreen.y = -vertexCamera.y / camera->hlen / vertexCamera.z;
	vertexNDC.x = (vertexScreen.x + 1) / 2;
	vertexNDC.y = (vertexScreen.y - 1) / -2;
	vertexRaster.x = vertexNDC.x * render->width - 0.5;
	vertexRaster.y = vertexNDC.y * render->height - 0.5;
	return (vertexRaster);
}

static void	render_pixel(t_thread_data *data, t_scene *scene, size_t x,
	size_t y)
{
	t_iterator	obj_iterator;
	t_ray		ray;
	t_object	*object;
	t_object	*obj_test;
	// t_ray		obj_ray;

	object = NULL;
	ray = compute_ray(scene->render, data->camera, x, y);
	obj_iterator = iterator_new(scene->objects);
	while (iterator_has_next(&obj_iterator))
	{
		obj_test = iterator_next(&obj_iterator);
		if (roundf(convertToRaster(scene->render, data->camera, obj_test->data.triangle.p1).x) == x
		&& roundf(convertToRaster(scene->render, data->camera, obj_test->data.triangle.p1).y) == y)
		{
			ray.color = color_new(255, 0, 0);
			object = obj_test;
		}
		if (roundf(convertToRaster(scene->render, data->camera, obj_test->data.triangle.p2).x) == x
		&& roundf(convertToRaster(scene->render, data->camera, obj_test->data.triangle.p2).y) == y)
		{
			ray.color = color_new(255, 0, 0);
			object = obj_test;
		}
		if (roundf(convertToRaster(scene->render, data->camera, obj_test->data.triangle.p3).x) == x
		&& roundf(convertToRaster(scene->render, data->camera, obj_test->data.triangle.p3).y) == y)
		{
			ray.color = color_new(255, 0, 0);
			object = obj_test;
		}
		// obj_ray = ray;
		// if (collision(obj_test, &obj_ray) && obj_ray.length < ray.length)
		// {
		// 	ray = obj_ray;
		// 	object = obj_test;
		// }
	}
	if (!object)
		ray.color = *(scene->background);
	// else
	// 	render_light(scene, object, &ray);
	data->vars->set_pixel(data->image, x, y, ray.color);
}

static void	render_chunk(t_thread_data *data, size_t start_x, size_t start_y)
{
	t_scene		*scene;
	size_t		x;
	size_t		y;

	scene = data->scene;
	x = start_x;
	while (x < start_x + scene->render->chunk_width && x < data->width)
	{
		y = start_y;
		while (y < start_y + scene->render->chunk_height && y < data->height)
		{
			render_pixel(data, scene, x, y);
			y++;
		}
		x++;
	}
}

void	*render_thread(t_thread_data *data, int *chunk)
{
	t_options	*params;
	int			chunk_x;
	int			chunk_y;
	int			ratio;

	params = data->scene->render;
	ratio = (int)ceilf(params->width / (float)params->chunk_width);
	chunk_x = *chunk % ratio * params->chunk_width;
	chunk_y = *chunk / ratio * params->chunk_height;
	render_chunk(data, chunk_x, chunk_y);
	pthread_mutex_lock(&(data->mutex_flush));
	if (log_msg(DEBUG, NULL))
	{
		printf("Rendering chunk (%d,%d)...", chunk_x, chunk_y);
		log_nl();
	}
	data->vars->on_refresh(data->vars, data->image);
	pthread_mutex_unlock(&(data->mutex_flush));
	return (NULL);
}
