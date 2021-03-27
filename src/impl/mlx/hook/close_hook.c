#include "minirt.h"

int	close_hook(t_vars *vars)
{
	exit_minirt(vars, NULL, NULL, EXIT_SUCCESS);
	return (TRUE);
}
