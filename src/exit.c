#include "minirt.h"

#include "tpool.h"

static void	free_camera_render(t_camera *camera, t_vars *vars)
{
	if (camera->z_buffer)
		free(camera->z_buffer);
	if (camera->render)
		vars->free_image(camera->render, vars);
}

void	exit_minirt(t_vars *vars, t_tpool *pool, void *other, int __status)
{
	lst_foreachp(get_scene()->cameras, (t_bicon)free_camera_render, vars);
	vars->on_exit(vars);
	tpool_free(pool);
	free_scene(get_scene());
	free(other);
	exit(__status);
}
