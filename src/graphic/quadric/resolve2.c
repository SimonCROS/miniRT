/**
 * @authors Lucille Burnet (lburnet@student.42lyon.fr),
 * Simon Cros (scros@student.42lyon.fr)
 */

#include <math.h>

#include "graphics.h"

/*
http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html
*/
t_vec3f	inter_quad_line_coeff(t_quadric *q, t_vec3f o, t_vec3f d)
{
	t_vec3f	abc;

	abc.x = q->a * powf(d.x, 2) + q->b * powf(d.y, 2) + q->c * powf(d.z, 2)
		+ q->d * d.x * d.y + q->e * d.x * d.z + q->f * d.y * d.z;
	abc.y = 2 * q->a * o.x * d.x + 2 * q->b * o.y * d.y + 2 * q->c * o.z * d.z
		+ q->d * (o.x * d.y + o.y * d.x) + q->e * (o.x * d.z + o.z * d.x)
		+ q->f * (o.y * d.z + o.z * d.y) + q->g * d.x + q->h * d.y + q->i * d.z;
	abc.z = q->a * powf(o.x, 2) + q->b * powf(o.y, 2) + q->c * powf(o.z, 2)
		+ q->d * o.x * o.y + q->e * o.x * o.z + q->f * o.y * o.z + q->g * o.x
		+ q->h * o.y + q->i * o.z + q->j;
	return (abc);
}

float	discriminant(t_vec3f abc)
{
	float	delta;

	if (abc.x == 0)
		return (0);
	delta = powf(abc.y, 2) - 4 * abc.x * abc.z;
	return (delta);
}

float	inter_quad_line_sol(t_vec3f abc, float d)
{
	float	t1;
	float	t2;

	if (abc.x == 0 && abc.y != 0)
		return (-abc.z / abc.y);
	if (d == 0)
		return (-abc.y * 0.5 / abc.x);
	t1 = (-abc.y - sqrtf(d)) * 0.5 / abc.x;
	t2 = (-abc.y + sqrtf(d)) * 0.5 / abc.x;
	if (t1 > 0 && t2 > 0)
		return (fminf(t1, t2));
	return (fmaxf(t1, t2));
}

void	inter_quad_line_sol_double(t_vec3f abc, float d, float t[2])
{
	float	t1;
	float	t2;

	t[1] = 0;
	if (abc.x == 0 && abc.y != 0)
		t[0] = -abc.z / abc.y;
	else if (d == 0)
		t[0] = -abc.y * 0.5 / abc.x;
	else
	{
		t1 = (-abc.y - sqrtf(d)) * 0.5 / abc.x;
		t2 = (-abc.y + sqrtf(d)) * 0.5 / abc.x;
		if (t1 > 0 && t2 > 0)
		{
			t[0] = fminf(t1, t2);
			t[1] = fmaxf(t1, t2);
		}
		else
			t[0] = fmaxf(t1, t2);
	}
	t[0] = fmaxf(t[0], 0);
}
