#include <math.h>
#include "libft.h"
#include "graphics.h"

t_line	line_fromf(t_vec2f v1, t_vec2f v2)
{
	t_line	line;

	line.from.x = v1.x;
	line.from.y = v1.y;
	line.to.x = v2.x;
	line.to.y = v2.y;
	return (line);
}

t_line	line_from(t_vec2i v1, t_vec2i v2)
{
	t_line	line;

	line.from.x = v1.x;
	line.from.y = v1.y;
	line.to.x = v2.x;
	line.to.y = v2.y;
	return (line);
}
