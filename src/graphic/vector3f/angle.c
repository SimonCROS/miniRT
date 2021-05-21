#include <math.h>
#include "graphics.h"

float	vec3_angle(t_vec3f v1, t_vec3f v2)
{
	return (acos(vec3_dot(v1, v2) / (vec3_length(v1) * vec3_length(v2))));
}
