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

	if (data->size != 4)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && ft_atof_full((char *)lst_get(data, 2), &brightness);
	e = e && color_deserialize((char *)lst_get(data, 3), &color);
	if (!e || brightness < 0 || brightness > 1)
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
