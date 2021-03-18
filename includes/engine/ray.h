#ifndef RAY_H
# define RAY_H

#include "libft.h"

# include "options.h"
# include "element/camera.h"

typedef struct s_ray {
	t_vector3		direction;
	t_vector3		phit;
	t_vector3		nhit;
	t_vector3		origin;
	t_color			color;
	float			length;
	float			light;
}	t_ray;

t_ray	compute_ray(t_options *render, t_camera *camera, float x, float y);

#endif
