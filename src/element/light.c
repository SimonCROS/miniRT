#include <stdlib.h>

#include "convert.h"
#include "matrix.h"
#include "vector3.h"

#include "element/light.h"
#include "util/parsing.h"

t_light	*parse_light(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	float		brightness;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !bounded_float_deserialize((char *)lst_get(data, 2), &brightness, 0,
			1)
		|| !col_deserialize((char *)lst_get(data, 3), &color)
		|| brightness < 0 || brightness > 1)
		return (NULL);
	return (new_light(brightness, vec3_addv(pos, origin), color));
}

t_light	*new_light(float brightness, t_vector3 position, t_color color)
{
	t_light	*light;

	light = malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->brightness = brightness;
	light->position = position;
	light->color = color;
	return (light);
}
