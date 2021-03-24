#include "minirt.h"

int	close_hook(t_vars *vars)
{
	return (on_close(vars));
}
