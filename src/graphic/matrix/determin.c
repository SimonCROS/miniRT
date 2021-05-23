#include <math.h>
#include "graphics.h"

static void	determin2(float matrix[4][4], float mt[4][4], int a, int k)
{
	int	v[4];

	v[2] = 0;
	v[3] = 0;
	v[0] = -1;
	while (++v[0] < k)
	{
		v[1] = -1;
		while (++v[1] < k)
		{
			mt[v[0]][v[1]] = 0;
			if ((v[0] != 0) && (v[1] != a))
			{
				mt[v[2]][v[3]] = matrix[v[0]][v[1]];
				if (v[3] < (k - 2))
					v[3]++;
				else
				{
					v[3] = 0;
					v[2]++;
				}
			}
		}
	}
}

float	determin(float matrix[4][4], int k)
{
	float	deter;
	float	z;
	float	mt[4][4];
	int		a;

	deter = 0;
	z = 1;
	if (k == 1)
		return (matrix[0][0]);
	else
	{
		deter = 0;
		a = -1;
		while (++a < k)
		{
			determin2(matrix, mt, a, k);
			deter = deter + z * (matrix[0][a] * determin(mt, k - 1));
			z = -1 * z;
		}
	}
	return (deter);
}
