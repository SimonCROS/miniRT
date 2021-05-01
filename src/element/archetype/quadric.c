#include "minirt.h"
#include "object.h"

t_object	*new_default_quadric(t_vector3 pos, t_vector3 rot, t_color col,
	t_bipredicate collides)
{
	t_object	*object;

	object = new_default_object(pos, rot, col, collides);
	if (!object)
		return (NULL);
	object->is_quadric = 1;
	return (object);
}
