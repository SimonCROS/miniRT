#include "minirt.h"

void	draw_line(t_vars *vars, int x0, int y0, int x1, int y1, t_color color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;
	int	w;
	int	h;

	w = get_scene()->render->width;
	h = get_scene()->render->height;
	dx = abs(x1 - x0);
	sx = 2 * (x0 < x1) - 1;
	dy = -abs(y1 - y0);
	sy = 2 * (y0 < y1) - 1;
	err = dx + dy;
	while (TRUE)
	{
		if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h)
			vars->set_pixel(vars->camera->render, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}