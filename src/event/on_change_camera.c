#include "listener/events.h"

#include "minirt.h" // TEMP
int	on_change_camera(t_vars *vars)
{
	return (render(vars));
}
