#include <math.h>
#include "graphics.h"

t_vec3f	vec3_rotate_x(t_vec3f vec, float theta)
{
	t_vec3f	res;

	res.x = vec.x;
	res.y = vec.y * cos(theta) - vec.z * sin(theta);
	res.z = vec.y * sin(theta) + vec.z * cos(theta);
	return (res);
}

t_vec3f	vec3_rotate_y(t_vec3f vec, float theta)
{
	t_vec3f	res;

	res.x = vec.x * cos(theta) + vec.z * sin(theta);
	res.y = vec.y;
	res.z = -vec.x * sin(theta) + vec.z * cos(theta);
	return (res);
}

t_vec3f	vec3_rotate_z(t_vec3f vec, float theta)
{
	t_vec3f	res;

	res.x = vec.x * cos(theta) - vec.y * sin(theta);
	res.y = vec.x * sin(theta) + vec.y * cos(theta);
	res.z = vec.z;
	return (res);
}

t_vec3f	vec3_rotate_axis(t_vec3f vec, t_vec3f a, float theta)
{
	t_vec3f	res;

	res.x = a.x * (a.x * vec.x + a.y * vec.y + a.z * vec.z) * (1 - cos(theta))
		+ vec.x * cos(theta)
		+ (-a.z * vec.y + a.y * vec.z) * sin(theta);
	res.y = a.y * (a.x * vec.x + a.y * vec.y + a.z * vec.z) * (1 - cos(theta))
		+ vec.y * cos(theta)
		+ (a.z * vec.x - a.x * vec.z) * sin(theta);
	res.z = a.z * (a.x * vec.x + a.y * vec.y + a.z * vec.z) * (1 - cos(theta))
		+ vec.z * cos(theta)
		+ (-a.y * vec.x + a.x * vec.y) * sin(theta);
	return (res);
}
