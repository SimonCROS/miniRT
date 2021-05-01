#include "minirt.h"
#include "object.h"

t_light	*new_default_light(t_vector3 position, t_color color, float brightness)
{
	t_light	*light;

	light = malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->position = position;
	light->color = color;
	light->brightness = brightness;
	light->calculate_ray = NULL;
	light->calculate_color = NULL;
	return (light);
}
