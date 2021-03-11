#ifndef LIGHT_H
# define LIGHT_H

# include "vector3.h"
# include "color.h"
# include "list.h"

typedef struct s_light
{
	float		brightness;
	t_vector3	position;
	t_color		color;
}	t_light;

t_light	*parse_light(t_list *data, t_vector3 origin);
t_light	*new_light(float brightness, t_vector3 position, t_color color);

#endif
