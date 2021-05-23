#include "graphics.h"

t_vec3f	mat44_mul_vec(t_matrix44 m, t_vec3f in)
{
	t_vec3f	out;

	out.x = in.x * m.e[0][0] + in.y * m.e[1][0] + in.z * m.e[2][0] + m.e[3][0];
	out.y = in.x * m.e[0][1] + in.y * m.e[1][1] + in.z * m.e[2][1] + m.e[3][1];
	out.z = in.x * m.e[0][2] + in.y * m.e[1][2] + in.z * m.e[2][2] + m.e[3][2];
	return (out);
}
