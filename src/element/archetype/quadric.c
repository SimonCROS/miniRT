#include "minirt.h"
#include "object.h"

t_object	*new_default_quadric(t_vec3f pos, t_vec3f rot, t_color col,
	t_bipredicate collides)
{
	t_object	*object;

	object = new_default_object(pos, rot, col, collides);
	if (!object)
		return (NULL);
	object->is_quadric = 1;
	return (object);
}
