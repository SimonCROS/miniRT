#include "minirt.h"
#include "object.h"

t_object	*new_default_object(t_vec3f pos, t_vec3f rot, t_color col,
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
	object->post_light_calculation = NULL;
	return (object);
}
