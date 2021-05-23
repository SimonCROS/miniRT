#include <math.h>
#include "graphics.h"

static float	determin(float matrix[4][4], int k)
{
	float	deter;
	float	z;
	float	mt[4][4];
	int		v[5];

	deter = 0;
	z = 1;
	if (k == 1)
		return (matrix[0][0]);
	else
	{
		deter = 0;
		v[0] = -1;
		while (++v[0] < k)
		{
			v[3] = 0;
			v[4] = 0;
			v[1] = -1;
			while (++v[1] < k)
			{
				v[2] = -1;
				while (++v[2] < k)
				{
					mt[v[1]][v[2]] = 0;
					if ((v[1] != 0) && (v[2] != v[0]))
					{
						mt[v[3]][v[4]] = matrix[v[1]][v[2]];
						if (v[4] < (k - 2))
							v[4]++;
						else
						{
							v[4] = 0;
							v[3]++;
						}
					}
				}
			}
			deter = deter + z * (matrix[0][v[0]] * determin(mt, k - 1));
			z = -1 * z;
		}
	}
	return (deter);
}

static void	trans(t_matrix44 *result, float matr[4][4], float m1[4][4], int r)
{
	float	tran[4][4];
	float	d;
	int		i;
	int		j;

	i = -1;
	while (++i < r)
	{	
		j = -1;
		while (++j < r)
			tran[i][j] = m1[j][i];
	}
	d = determin(matr, r);
	i = -1;
	while (++i < r)
	{	
		j = -1;
		while (++j < r)
			result->e[i][j] = tran[i][j] / d;
	}
}

static void	cofac(t_matrix44 *result, float comatr[4][4], int f)
{
	float	matr[4][4];
	float	cofact[4][4];
	int		v[6];

	v[2] = -1;
	while (++v[2] < f)
	{
		v[3] = -1;
		while (++v[3] < f)
		{
			v[4] = 0;
			v[5] = 0;
			v[0] = -1;
			while (++v[0] < f)
			{
				v[1] = -1;
				while (++v[1] < f)
				{
					if (v[0] != v[2] && v[1] != v[3])
					{
						matr[v[4]][v[5]] = comatr[v[0]][v[1]];
						if (v[5] < (f - 2))
							v[5]++;
						else
						{
							v[5] = 0;
							v[4]++;
						}
					}
				}
			}
			cofact[v[2]][v[3]] = pow(-1, v[2] + v[3]) * determin(matr, f - 1);
		}
	}
	trans(result, comatr, cofact, f);
}

t_matrix44	mat44_inverse(t_matrix44 m)
{
	float		deter;
	t_matrix44	result;

	deter = determin(m.e, 4);
	if (deter == 0)
		result = mat44_null();
	else
		cofac(&result, m.e, 4);
	return (result);
}
