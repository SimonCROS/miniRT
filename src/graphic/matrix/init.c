#include "graphics.h"

t_matrix44	mat44_null(void)
{
	t_matrix44	out;
	int			i;
	int			j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			out.e[i][j] = 0;
	}
	return (out);
}
