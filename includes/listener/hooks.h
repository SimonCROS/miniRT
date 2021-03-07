#ifndef HOOKS_H
# define HOOKS_H

# include "display/window.h"
# include "util/scene.h"

int	key_hook(int key, t_vars *vars, t_scene *scene);
int	close_hook(t_vars *vars, t_scene *scene);

#endif
