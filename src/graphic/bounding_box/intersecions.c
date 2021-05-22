#include "libft.h"
#include "graphics.h"

t_bounding_box	bounding_box_intersection(t_bounding_box b1, t_bounding_box b2)
{
	t_bounding_box	box;

	box.min.x = fmaxi(b1.min.x, b2.min.x);
	box.min.y = fmaxi(b1.min.y, b2.min.y);
	box.max.x = fmini(b1.max.x, b2.max.x);
	box.max.y = fmini(b1.max.y, b2.max.y);
	return (box);
}

#include <stdio.h>

int	bounding_box_intersects(t_bounding_box b1, t_bounding_box b2)
{
	if (b1.min.x == b1.max.x || b1.min.y == b2.max.y || b2.min.x == b2.max.x
		|| b2.min.y == b2.max.y)
		return (FALSE);
	if (b1.min.x >= b2.max.x || b2.min.x >= b1.max.x)
		return (FALSE);
	if (b1.min.y >= b2.max.y || b2.min.y >= b1.max.y)
		return (FALSE);
	return (TRUE);
}
