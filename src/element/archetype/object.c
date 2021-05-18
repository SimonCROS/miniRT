#include "minirt.h"
#include "object.h"

static t_color	default_calc_color(t_camera *c, t_object *o, t_ray *r)
{
	(void)c;
	(void)r;
	return (o->color);
}

t_object	*new_default_object(t_vector3 pos, t_vector3 rot, t_color col,
	t_bipredicate collides)
{
	t_object	*object;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	object->position = pos;
	object->rotation = vec3_normalize(rot);
	object->collides = collides;
	object->color = col;
	object->is_plane = 0;
	object->is_quadric = 0;
	object->calculate_color = (t_calc_color)default_calc_color;
	return (object);
}
