#include "minirt.h"

static t_bitmap	*bmp_init_image(t_vars *vars, t_options *params)
{
	(void)vars;
	return (bmp_init(params->width, params->height));
}

static void	bmp_free_image(t_bitmap *image, t_vars *vars)
{
	if (image)
	{
		(void)vars;
		free(image->body);
		free(image);
	}
}

static void	bmp_finished(t_vars *vars, t_camera *camera)
{
	t_bitmap	*image;

	(void)vars;
	image = camera->render;
	log_msg(INFO, "Saving...");
	if (!bmp_save("render.bmp", image))
	{
		log_msg(DEBUG, "Error while saving image to bmp.");
		perror("Error\nError while saving image to bmp");
	}
	else
		log_msg(INFO, "Saved !");
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
	vars.free_image = (t_bicon)bmp_free_image;
	vars.on_exit = null_consumer();
	render(&vars);
	exit_minirt(&vars, NULL, NULL, EXIT_SUCCESS);
}
