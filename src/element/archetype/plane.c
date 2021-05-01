#include "minirt.h"
#include "object.h"

t_object	*new_default_plane(t_vector3 pos, t_vector3 rot, t_color col,
	t_bipredicate collides)
{
	t_object	*plan;

	plan = new_default_object(pos, rot, col, collides);
	if (!plan)
		return (NULL);
	plan->is_plane = 1;
	return (plan);
}
