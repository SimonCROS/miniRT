#include <math.h>
#include "libft.h"
#include "graphics.h"

t_bounding_box	bounding_box_fromf3(t_vec2f v1, t_vec2f v2, t_vec2f v3)
{
	t_bounding_box	box;

	box.min.x = floorf(fminf(v1.x, fminf(v2.x, v3.x)));
	box.min.y = floorf(fminf(v1.y, fminf(v2.y, v3.y)));
	box.max.x = ceilf(fmaxf(v1.x, fmaxf(v2.x, v3.x)));
	box.max.y = ceilf(fmaxf(v1.y, fmaxf(v2.y, v3.y)));
	return (box);
}

t_bounding_box	bounding_box_fromf(t_vec2f v1, t_vec2f v2)
{
	t_bounding_box	box;

	box.min.x = floorf(fminf(v1.x, v2.x));
	box.min.y = floorf(fminf(v1.y, v2.y));
	box.max.x = ceilf(fmaxf(v1.x, v2.x));
	box.max.y = ceilf(fmaxf(v1.y, v2.y));
	return (box);
}

t_bounding_box	bounding_box_from3(t_vec2i v1, t_vec2i v2, t_vec2i v3)
{
	t_bounding_box	box;

	box.min.x = fmini3(v1.x, v2.x, v3.x);
	box.min.y = fmini3(v1.y, v2.y, v3.y);
	box.max.x = fmaxi3(v1.x, v2.x, v3.x);
	box.max.y = fmaxi3(v1.y, v2.y, v3.y);
	return (box);
}

t_bounding_box	bounding_box_from(t_vec2i v1, t_vec2i v2)
{
	t_bounding_box	box;

	box.min.x = fmini(v1.x, v2.x);
	box.min.y = fmini(v1.y, v2.y);
	box.max.x = fmaxi(v1.x, v2.x);
	box.max.y = fmaxi(v1.y, v2.y);
	return (box);
}
