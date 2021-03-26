#include "minirt.h"

#include "tpool.h"

void	exit_minirt(t_vars *vars, t_tpool *pool, void *other, int __status)
{
	lst_foreachp(get_scene()->cameras, vars->free_image, vars);
	vars->on_exit(vars);
	tpool_destroy(pool);
	free_scene(get_scene());
	free(other);
	exit(__status);
}
