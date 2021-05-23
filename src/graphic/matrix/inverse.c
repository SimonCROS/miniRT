#include <math.h>
#include "graphics.h"

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

static void	cofac2(float matr[4][4], float comatr[4][4], int a[2], int f)
{
	int	v[4];

	v[2] = 0;
	v[3] = 0;
	v[0] = -1;
	while (++v[0] < f)
	{
		v[1] = -1;
		while (++v[1] < f)
		{
			if (v[0] != a[0] && v[1] != a[1])
			{
				matr[v[2]][v[3]] = comatr[v[0]][v[1]];
				if (v[3] < (f - 2))
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

static void	cofac(t_matrix44 *result, float comatr[4][4], int f)
{
	float	matr[4][4];
	float	cofact[4][4];
	int		a[2];

	a[0] = -1;
	while (++a[0] < f)
	{
		a[1] = -1;
		while (++a[1] < f)
		{
			cofac2(matr, comatr, a, f);
			cofact[a[0]][a[1]] = pow(-1, a[0] + a[1]) * determin(matr, f - 1);
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
