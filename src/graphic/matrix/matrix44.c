#include <math.h>
#include "graphics.h"

t_vec3f	mat44_mul_vec(t_matrix44 m, t_vec3f in)
{
	t_vec3f	out;

	out.x = in.x * m.e[0][0] + in.y * m.e[1][0] + in.z * m.e[2][0] + m.e[3][0];
	out.y = in.x * m.e[0][1] + in.y * m.e[1][1] + in.z * m.e[2][1] + m.e[3][1];
	out.z = in.x * m.e[0][2] + in.y * m.e[1][2] + in.z * m.e[2][2] + m.e[3][2];
	return (out);
}

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

static float	determin(float matrix[4][4], int k)
{
	float	deter = 0.0, z = 1.0, mt[4][4];
	int		a, b, c, x, y;

	if (k == 1)
		return (matrix[0][0]);
	else
	{
		deter = 0;
		for (a = 0; a < k; ++a) {
			x = 0;
			y = 0;
			for (b = 0; b < k; ++b) {
				for (c = 0; c < k; ++c) {
					mt[b][c] = 0;
					if ((b != 0) && (c != a)) {
						mt[x][y] = matrix[b][c];
						if (y < (k - 2))
							y++;
						else {
							y = 0;
							x++;
						}
					}
				}
			}
			deter = deter + z * (matrix[0][a] * determin(mt, k - 1));
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

static void cofac(t_matrix44 *result, float comatr[4][4], int f) {
	float	matr[4][4], cofact[4][4];
	int		a, b, c, d, x, y;

	for (c = 0; c < f; ++c) {
		for (d = 0; d < f; ++d) {
			x = 0;
			y = 0;
			for (a = 0; a < f; ++a) {
				for (b = 0; b < f; ++b) {
					if (a != c && b != d) {
						matr[x][y] = comatr[a][b];
						if (y < (f - 2))
							y++;
						else {
							y = 0;
							x++;
						}
					}
				}
			}
			cofact[c][d] = pow(-1, c + d) * determin(matr, f - 1);
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
