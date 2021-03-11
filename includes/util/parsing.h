#ifndef PARSING_H
# define PARSING_H

# include "vector3.h"
# include "util/scene.h"

int		parse_file(t_scene *scene, char *file, int depth, t_vector3 origin);
int		vec_deserialize(const char *str, t_vector3 *vector);
int		dir_deserialize(const char *str, t_vector3 *vector);
int		float_deserialize(const char *str, float *f);
int		int_deserialize(const char *str, int *f);
t_scene	*parse(char *file);

#endif
