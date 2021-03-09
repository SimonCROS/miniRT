#include <math.h>
#include <pthread.h>

#include "mlx.h"

#include "matrix.h"
#include "bitmap.h"
#include "tpool.h"
#include "list.h"
#include "iterator.h"
#include "ftstring.h"

#include "minirt.h"
#include "options.h"
#include "element/camera.h"
#include "element/light.h"
#include "element/plan.h"
#include "engine/ray.h"
#include "engine/render_thread.h"
#include "provider/scene_provider.h"
#include "display/window.h"
#include "util/logs.h"

static pthread_mutex_t	g_mutex_flush = PTHREAD_MUTEX_INITIALIZER;

void	render_chunk(t_thread_data *data, size_t start_x, size_t start_y)
{
	t_iterator	objectIterator;
	t_iterator	lightIterator;

	objectIterator = iterator_new(data->scene->objects);
	lightIterator = iterator_new(data->scene->lights);
	for (size_t x = start_x; x < start_x + data->scene->render->chunk_width; x++)
	{
		if (x >= data->width)
			break;
		for (size_t y = start_y; y < start_y + data->scene->render->chunk_height; y++)
		{
			if (y >= data->height)
				break;

			t_ray	ray = compute_ray(data->scene->render, data->camera, x, y);
			t_object	*object = NULL;
			t_object	*object_test;
			t_ray	obj_ray;

			while (iterator_has_next(&objectIterator))
			{
				object_test = iterator_next(&objectIterator);
				obj_ray = ray;

				if (collision(object_test, &obj_ray))
				{
					if (obj_ray.length < ray.length)
					{
						ray = obj_ray;
						object = object_test;
					}
				}
			}

			iterator_reset(&objectIterator);

			if (!object)
				continue;
			ray.color = color_mul(object->color, *(data->scene->ambiant));

			while (iterator_has_next(&lightIterator))
			{
				t_light *light = iterator_next(&lightIterator);

				t_vector3 lightDir = vec3_subv(light->position, ray.phit);
				float lightDistance2 = vec3_length_squared(lightDir); 
				lightDir = vec3_normalize(lightDir);
				float LdotN = fmaxf(0, vec3_dotv(lightDir, ray.nhit));
				short inShadow = FALSE;

				t_ray light_ray;
				light_ray.origin = ray.phit;
				light_ray.direction = lightDir;
				light_ray.length = INFINITY;
				while (iterator_has_next(&objectIterator))
				{
					t_object *obj_test = iterator_next(&objectIterator);
					if (obj_test == object)
						continue;
					if ((inShadow = collision(obj_test, &light_ray)))
					{
						if (light_ray.length * light_ray.length > lightDistance2)
							inShadow = 0;
						else
							break;
					}
				}
				iterator_reset(&objectIterator);
				float atm = (1 - inShadow) * light->brightness * LdotN;
				ray.color = color_add(ray.color, color_mul(object->color, color_mulf(color_mulf(light->color, light->brightness), atm)));
			}
			iterator_reset(&lightIterator);
			data->vars->set_pixel(data->image, x, y, ray.color);
		}
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
	pthread_mutex_lock(&g_mutex_flush);
	if (log_msg(DEBUG, NULL))
	{
		printf("Rendering chunk (%d,%d)...", chunk_x, chunk_y);
		log_nl();
	}
	data->vars->on_refresh(data->vars, data->image);
	pthread_mutex_unlock(&g_mutex_flush);
	return (NULL);
}

t_bitmap	*bmp_init_image(t_vars *vars, t_options *params)
{
	(void)vars;
	return (bmp_init(params->width, params->height));
}

void	bmp_finished(t_camera *camera, t_bitmap *image)
{
	(void)camera;
	bmp_save("render.bmp", image);
	free(image);
}

int	render2(t_vars *vars, t_camera *camera, t_scene *scene)
{
	t_tpool			*pool;
	t_thread_data	data;
	t_options		*params;
	size_t			*chunks;
	size_t			chunk;

	params = scene->render;
	data.vars = vars;
	data.image = vars->init_image(vars, params);
	data.width = params->width;
	data.height = params->height;
	data.camera = camera;
	data.scene = scene;
	data.chunks = ceilf(params->width / (float)params->chunk_width)
		* ceilf(params->height / (float)params->chunk_height);
	pool = tpool_new(params->threads);
	chunks = malloc(data.chunks * sizeof(size_t));
	if (!chunks)
		return (FALSE);
	chunk = 0;
	while (chunk < data.chunks)
	{
		chunks[chunk] = chunk;
		tpool_add_work(pool, (t_bifun)render_thread, &data, chunks + chunk);
		chunk++;
	}
	tpool_set_name(pool, "CHUNK_WORKER");
	tpool_start(pool);
	tpool_wait(pool);
	tpool_destroy(pool);
	free(chunks);
	log_msg(INFO, "Image successfully rendered");
	vars->on_finished(camera, data.image);
	return (TRUE);
}

int		render(t_vars *vars)
{
	t_scene		*scene;
	t_camera	*camera;

	scene = get_scene();
	camera = lst_get(scene->cameras, scene->index);
	if (camera->render)
	{
		vars->on_refresh(vars, camera->render);
		log_msg(DEBUG, "Image loaded from cache");
		return (0);
	}
	return (render2(vars, camera, scene));
}

void	load_image(char *file, t_scene *scene)
{
	t_vars		vars;

	(void)file;
	(void)scene;
	vars.init_image = (t_bifun)bmp_init_image;
	vars.set_pixel = (t_pixel_writer)bmp_set_pixel;
	vars.on_refresh = null_biconsumer();
	vars.on_finished = (t_bicon)bmp_finished;
	render(&vars);
}

int main(int argc, char **argv)
{
	int		save;

	pthread_setname_np("MAIN");
	log_msg(INFO, "Launching");
	save = 0;
	if (argc != 2)
	{
		if (argc == 3 && !ft_strcmp(argv[2], "--save"))
			save = 1;
		else
			return (0);
	}
	if (!load_scene(argv[1]))
		exit(1); // TODO
	if (save)
		load_image(argv[1], get_scene());
	else
		init_window(argv[1], get_scene());
	pthread_exit(NULL);
}
