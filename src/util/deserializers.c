#include "vector3.h"

int	dir_deserialize(const char *str, t_vector3 *vector)
{
	t_vector3	rot;

	if (!vec3_deserialize(str, &rot))
		return (0);
	if (rot.x < -1 || rot.x > 1 || rot.y < -1 || rot.y > 1
		|| rot.z < -1 || rot.z > 1)
		return (0);
	*vector = rot;
	return (1);
}
