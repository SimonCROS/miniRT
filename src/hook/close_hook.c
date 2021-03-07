#include "provider/scene_provider.h"
#include "display/window.h"
#include "listener/events.h"

int	close_hook(t_vars *vars)
{
	return (on_close(vars, get_scene()));
}
