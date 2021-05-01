#include "minirt.h"
#include "object.h"

#include <math.h>

t_object	*parse_plane(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	t_vector3	rot;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !col_deserialize((char *)lst_get(data, 3), &color))
		return (NULL);
	return (new_plane(vec3_addv(pos, origin), rot, color));
}

t_object	*new_plane(t_vector3 pos, t_vector3 rot, t_color col)
{
	return (new_default_plane(pos, rot, col, NULL));
}
