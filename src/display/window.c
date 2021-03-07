#include "mlx.h"

#include "display/image.h"
#include "display/window.h"
#include "element/camera.h"
#include "listener/hooks.h"

#ifdef __linux__
# include <X11/Xlib.h>
#endif

void	mlx_finished(t_camera *camera, t_image *image)
{
	camera->render = image;
}

void	force_put_image(t_vars *vars, t_image *image)
{
#if defined __APPLE__
	mlx_sync(MLX_SYNC_WIN_FLUSH_CMD, vars->win);
#elif defined __linux__
	*(int*)(vars->mlx + 80) = 1;
#endif
	mlx_put_image_to_window(vars->mlx, vars->win, image->img, 0, 0);
}

void	init_window(char *file, t_scene *scene)
{
	char	*name;
	t_vars	vars;

#ifdef __linux__
	XInitThreads();
#endif
	vars.mlx = mlx_init();
	if (!vars.mlx)
	{
		printf("Error, can't generate the frame\n");
		exit(1);  // TODO
	}
	name = ft_strjoin("MiniRT - ", file);
	if (!name)
		exit(1);  // TODO
	vars.win = mlx_new_window(vars.mlx, scene->render->width,
			scene->render->height, name);
	vars.init_image = (t_bifun)mlx_init_image;
	vars.set_pixel = (t_pixel_writer)mlx_set_pixel;
	vars.on_refresh = (t_bicon)force_put_image;
	vars.on_finished = (t_bicon)mlx_finished;
	free(name);
	mlx_hook(vars.win, 17, 0L, &close_hook, &vars);
	mlx_key_hook(vars.win, &key_hook, &vars);
	mlx_string_put(vars.mlx, vars.win, 0, 50, ~0, "Press any key to start");
	mlx_loop(vars.mlx);
}
