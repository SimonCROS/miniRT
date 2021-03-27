#include "minirt.h"

int	vec_deserialize(const char *str, t_vector3 *vector)
{
	t_vector3	rot;

	if (!vec3_deserialize(str, &rot))
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unable to parse the following part into a vector \"%s\"", str);
		log_nl();
		return (0);
	}
	*vector = rot;
	return (1);
}

int	dir_deserialize(const char *str, t_vector3 *direction)
{
	t_vector3	dir;

	if (!vec_deserialize(str, &dir))
		return (0);
	if (dir.x < -1 || dir.x > 1 || dir.y < -1 || dir.y > 1
		|| dir.z < -1 || dir.z > 1)
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unable to parse the following part into a direction \"%s\"", str);
		log_nl();
		return (0);
	}
	*direction = vec3_normalize(dir);
	return (1);
}

int	col_deserialize(const char *str, t_color *color)
{
	t_color	col;

	if (!color_deserialize(str, &col))
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unable to parse the following part into a color \"%s\"", str);
		log_nl();
		return (0);
	}
	*color = col;
	return (1);
}

int	args_size(const char *type, int given, int expected)
{
	if (given != expected)
	{
		errno = 7;
		log_msg(ERROR, NULL);
		if (given > expected)
			printf("Too many arguments for type \"%s\" : %d arguments \
expected, %d given", type, expected, given);
		else
			printf("Not enough arguments for type \"%s\" : %d arguments \
expected, %d given", type, expected, given);
		log_nl();
		return (0);
	}
	return (1);
}
