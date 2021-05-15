#include "minirt.h"

int	on_change_camera(t_vars *vars, t_camera *from, t_camera *to)
{
	if (from)
		from->sepia = FALSE;
	(void)to;
	return (render(vars));
}
