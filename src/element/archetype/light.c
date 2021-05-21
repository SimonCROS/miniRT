#include <math.h>

#include "minirt.h"
#include "object.h"

static t_color	calculate_color(t_light *light, t_object *object,
	const t_ray *ray, float angle)
{
	(void)ray;
	return (
		color_mul(object->color,
			color_mulf(light->color,
				light->brightness * light->brightness * angle
			)
		)
	);
}

t_light	*new_default_light(t_vec3f position, t_color color, float brightness)
{
	t_light	*light;

	light = malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->position = position;
	light->color = color;
	light->brightness = brightness;
	light->calculate_ray = NULL;
	light->calculate_color = calculate_color;
	return (light);
}
