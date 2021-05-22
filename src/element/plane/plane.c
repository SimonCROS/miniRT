#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_plane(t_list *data, t_vec3f origin)
{
	t_vec3f	pos;
	t_vec3f	rot;
	t_color	color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !col_deserialize((char *)lst_get(data, 3), &color))
		return (NULL);
	return (new_plane(vec3_add(pos, origin), rot, color));
}

t_object	*new_plane(t_vec3f pos, t_vec3f rot, t_color col)
{
	return (new_default_plane(pos, rot, col, NULL));
}
