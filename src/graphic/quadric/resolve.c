/**
 * @authors Lucille Burnet (lburnet@student.42lyon.fr),
 * Simon Cros (scros@student.42lyon.fr)
 */

#include <math.h>

#include "graphics.h"

float	resolve_quad(t_quadric *quad, t_vec3f o, t_vec3f dir)
{
	t_vec3f	abc;
	float	d;
	float	t;

	abc = inter_quad_line_coeff(quad, o, dir);
	d = discriminant(abc);
	t = inter_quad_line_sol(abc, d);
	return (fmaxf(t, 0));
}

int	resolve_quad_double(t_quadric *quad, t_vec3f o, t_vec3f di, float *t)
{
	t_vec3f	abc;
	float	d;

	abc = inter_quad_line_coeff(quad, o, di);
	d = discriminant(abc);
	inter_quad_line_sol_double(abc, d, t);
	return (!!t[0]);
}

/*
Rn = [xn yn zn] = [dF/dx dF/dy dF/dz]

which gives
xn = 2*A*xi + D*yi + E*zi + G
yn = 2*B*yi + D*xi + F*zi + H
zn = 2*C*zi + E*xi + F*yi + I

Rn must be normalized and also we have to find the normal for surface facing the
 ray.
If R *Rd > 0 then reverse Rn.
*/
t_vec3f	resolve_quad_norm(t_quadric *qua, t_vec3f phit)
{
	t_vec3f	norm;

	norm.x = 2 * qua->a * phit.x + qua->d * phit.y + qua->e * phit.z + qua->g;
	norm.y = 2 * qua->b * phit.y + qua->d * phit.x + qua->f * phit.z + qua->h;
	norm.z = 2 * qua->c * phit.z + qua->e * phit.x + qua->f * phit.y + qua->i;
	return (vec3_normalize(norm));
}
