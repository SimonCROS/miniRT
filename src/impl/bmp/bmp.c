#include "minirt.h"

static t_bitmap	*bmp_init_image(t_vars *vars, t_options *params)
{
	(void)vars;
	return (bmp_init(params->width, params->height));
}

static void	bmp_finished(t_vars *vars, t_camera *camera)
{
	t_bitmap	*image;

	(void)vars;
	image = camera->render;
	log_msg(INFO, "Saving...");
	(void)camera;
	if (!bmp_save("render.bmp", image))
	{
		log_msg(DEBUG, "Error while saving image to bmp.");
		perror("Error\nError while saving image to bmp");
	}
	free(image->body);
	free(image);
	free_scene(get_scene());
}

void	init_bmp_image(char *file, t_scene *scene)
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
