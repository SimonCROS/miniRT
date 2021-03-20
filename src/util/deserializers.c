#include <errno.h>

#include "minirt.h"

int	float_deserialize(char *str, float *result)
{
	float	res;

	if (!ft_atof_full(str, &res))
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unable to parse the following part into a float \"%s\"", str);
		log_nl();
		return (0);
	}
	*result = res;
	return (1);
}

int	bounded_float_deserialize(char *str, float *result, float min, float max)
{
	float	res;

	if (!float_deserialize(str, &res))
		return (0);
	if (res < min || res > max)
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("The following float must be between %f and %f \"%s\"",
			min, max, str);
		log_nl();
		return (0);
	}
	*result = res;
	return (1);
}

int	int_deserialize(char *str, int *result)
{
	int	res;

	if (!ft_atoi_full(str, &res))
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unable to parse the following part into an int \"%s\"", str);
		log_nl();
		return (0);
	}
	*result = res;
	return (1);
}

int	bounded_int_deserialize(char *str, int *result, int min, int max)
{
	int	res;

	if (!int_deserialize(str, &res))
		return (0);
	if (res < min || res > max)
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("The following int must be between %d and %d \"%s\"",
			min, max, str);
		log_nl();
		return (0);
	}
	*result = res;
	return (1);
}

int	ulong_deserialize(char *str, unsigned long *result)
{
	unsigned long	res;

	if (!ft_atoul_full(str, &res))
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unable to parse the following part into an unsigned long \
\"%s\"", str);
		log_nl();
		return (0);
	}
	*result = res;
	return (1);
}
