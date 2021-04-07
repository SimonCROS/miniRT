#include "minirt.h"

int	mouse_pressed_hook(int b, t_vars *vars)
{
	(void)vars;
	log_msg(INFO, "Meuh");
	printf("- %d\n", b);
	return (0);
}

int	mouse_released_hook(int b, t_vars *vars)
{
	(void)vars;
	log_msg(INFO, "Moeu");
	printf("- %d\n", b);
	return (0);
}
