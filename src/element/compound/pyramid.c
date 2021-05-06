#include "minirt.h"
#include "object.h"

#include <math.h>

static int	generate_triangles(t_list *objects, float wh[2], t_vector3 pr[2],
	t_color c)
{
	t_object	*obj;
	t_square	square;
	t_vector3	top;
	float		w;

	w = wh[0];
	top = vec3_addv(pr[0], vec3_muld(pr[1], wh[1]));
	obj = new_square(w, vec3_addv(pr[0], vec3_muld(pr[1], w / 2)), pr[1], c);
	if (!lst_unshift(objects, obj))
		return (FALSE);
	square = obj->data.square;
	if (!lst_unshift(objects, new_triangle(square.p1, square.p2, top, c))
		|| !lst_unshift(objects, new_triangle(square.p2, square.p3, top, c))
		|| !lst_unshift(objects, new_triangle(square.p3, square.p4, top, c))
		|| !lst_unshift(objects, new_triangle(square.p4, square.p1, top, c)))
		return (FALSE);
	return (TRUE);
}

int	parse_pyramid(t_list *data, t_vector3 origin, t_list *objects)
{
	t_vector3	pos;
	t_vector3	rot;
	float		width;
	float		height;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 5))
		return (0);
	if (!vec3_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &width)
		|| !float_deserialize((char *)lst_get(data, 4), &height)
		|| !color_deserialize((char *)lst_get(data, 5), &color))
		return (0);
	width = fabsf(width);
	return (generate_triangles(objects, (float [2]){width, height},
			(t_vector3 [2]){vec3_addv(pos, origin), rot}, color));
}
