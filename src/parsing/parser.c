#include "minirt.h"
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

float	ft_atof(const char **str)
{
	int point;
	float ret;
	float mul;

	ret = 0;
	mul = 1;
	point = 0;
	if (*((*str)++) == '-')
		mul = -1;
	while ((**str >= '0' && **str <= '9') || (**str == '.' && !point))
	{
		if (**str == '.')
			point = 1;
		else
		{
			if (point)
				mul /= 10.0f;
			ret = ret * 10.0f + (float)(**str - '0');
		}
		(*str)++;
	}
	return (ret * mul);
}

t_scene	*parse(char *file)
{
	int		fd;
	char	*line;

	fd = open(file, O_RDONLY);
	line = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		printf("%s\n", line);
	}
	if (line)
		printf("%s\n", line);
	return (NULL);
}
