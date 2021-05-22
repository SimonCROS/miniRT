#include <math.h>
#include <unistd.h>

void	fill_z_buff(float *z_buffer, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		z_buffer[i] = INFINITY;
		i++;
	}
}

float	*get_z_buffer_value(float *z_buffer, int x, int y, int width)
{
	return (z_buffer + y * width + x);
}
