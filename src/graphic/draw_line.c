#include "minirt.h"
#include "graphics.h"

void	draw_line(t_vars *vars, t_line ln, t_color color)
{
	t_vec2i	d;
	t_vec2i	s;
	int		err;
	int		e2;

	d = (t_vec2i){abs(ln.to.x - ln.from.x), -abs(ln.to.y - ln.from.y)};
	s = (t_vec2i){2 * (ln.from.x < ln.to.x) - 1, 2 * (ln.from.y < ln.to.y) - 1};
	err = d.x + d.y;
	while (ln.from.x != ln.to.x || ln.from.y != ln.to.y)
	{
		if (ln.from.x >= 0 && ln.from.x < vars->scene->render->width
			&& ln.from.y >= 0 && ln.from.y < vars->scene->render->height)
			vars->set_pixel(vars->camera->render, ln.from.x, ln.from.y, color);
		e2 = 2 * err;
		if (e2 >= d.y)
		{
			err += d.y;
			ln.from.x += s.x;
		}
		if (e2 <= d.x)
		{
			err += d.x;
			ln.from.y += s.y;
		}
	}
}
