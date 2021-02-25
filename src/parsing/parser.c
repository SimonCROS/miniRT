#include "minirt.h"
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

float stof(const char* s)
{
	int point;
	float ret;
	float mul;

	ret = 0;
	mul = 1;
	if (*(s++) == '-')
		mul = -1;
	while ((*s >= '0' && *s <= '9') || (*s == '.' && !point))
	{
		point = 0;
		if (*s == '.')
		{
			point = 1; 
			continue;
		}
		if (point)
			mul /= 10.0f;
		ret = ret * 10.0f + (float)(*s - '0');
		s++;
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
