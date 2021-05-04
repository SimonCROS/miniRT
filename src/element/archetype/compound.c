#include "minirt.h"
#include "object.h"

static int	collides_compound_object(t_object *container, t_ray *ray)
{
	return (!!nearest_object(container->data.compound.elements, ray));
}

t_object	*new_default_compound_object(t_vector3 p, t_vector3 r, t_color c)
{
	t_object	*object;

	object = new_default_object(p, r, c, (t_bipre)collides_compound_object);
	if (!object)
		return (NULL);
	object->data.compound.elements = lst_new(&free);
	if (!object->data.compound.elements)
	{
		free(object);
		return (NULL);
	}
	return (object);
}
