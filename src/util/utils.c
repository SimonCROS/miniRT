#include "minirt.h"

float	fminf3(float a, float b, float c)
{
	return (fminf(a, fminf(b, c)));
}

float	fmaxf3(float a, float b, float c)
{
	return (fmaxf(a, fmaxf(b, c)));
}

void	get_samples_template(int aa, void *buf)
{
	if (aa == 1)
		ft_memcpy(buf, (t_noaa){{0, 0}}, sizeof(t_noaa));
	else if (aa == 4)
		ft_memcpy(buf, (t_ssaa4){{0.25, 0.4}, {0.4, -0.25}, {-0.25, -0.4},
		{-0.4, 0.25}}, sizeof(t_ssaa4));
}

t_color	to_sepia(t_color in)
{
	t_color	out;

	out.r = (in.r * .393) + (in.g * .769) + (in.b * .189);
	out.g = (in.r * .349) + (in.g * .686) + (in.b * .168);
	out.b = (in.r * .272) + (in.g * .534) + (in.b * .131);
	out.a = 1;
	return (out);
}
