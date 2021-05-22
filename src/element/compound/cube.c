#include "minirt.h"
#include "object.h"

#include <math.h>

static t_object	*new_custom_square(t_vec3f points[4], t_color color)
{
	t_object	*square;
	t_vec3f		rotation;
	float		width;

	rotation = vec3_cross(vec3_sub(points[1], points[0]),
			vec3_sub(points[2], points[0]));
	width = vec3_distance_squared(points[1], points[2]);
	square = new_square(width, points[0], rotation, color);
	if (!square)
		return (NULL);
	square->data.square.p1 = points[0];
	square->data.square.p2 = points[1];
	square->data.square.p3 = points[2];
	square->data.square.p4 = points[3];
	return (square);
}

static int	generate_squares(t_list *objects, float w, t_vec3f pr[2],
	t_color c)
{
	t_object	*obj;
	t_square	top;
	t_square	bot;

	obj = new_square(w, vec3_add(pr[0], vec3_muld(pr[1], w / 2)), pr[1], c);
	if (!lst_unshift(objects, obj))
		return (FALSE);
	top = obj->data.square;
	obj = new_square(w, vec3_sub(pr[0], vec3_muld(pr[1], w / 2)), pr[1], c);
	if (!lst_unshift(objects, obj))
		return (FALSE);
	bot = obj->data.square;
	if (!lst_unshift(objects, new_custom_square((t_vec3f [4]){top.p1, top.p2,
				bot.p2, bot.p1}, c))
	|| !lst_unshift(objects, new_custom_square((t_vec3f [4]){top.p2, top.p3,
				bot.p3, bot.p2}, c))
	|| !lst_unshift(objects, new_custom_square((t_vec3f [4]){top.p3, top.p4,
				bot.p4, bot.p3}, c))
	|| !lst_unshift(objects, new_custom_square((t_vec3f [4]){top.p4, top.p1,
				bot.p1, bot.p4}, c)))
		return (FALSE);
	return (TRUE);
}

int	parse_cube(t_list *data, t_vec3f origin, t_list *objects)
{
	t_vec3f	pos;
	t_vec3f	rot;
	float	width;
	t_color	color;

	if (!args_size(lst_first(data), data->size, 5))
		return (0);
	if (!vec3_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !float_deserialize((char *)lst_get(data, 3), &width)
		|| !color_deserialize((char *)lst_get(data, 4), &color))
		return (0);
	width = fabsf(width);
	return (generate_squares(objects, width,
			(t_vec3f [2]){vec3_add(pos, origin), rot}, color));
}
