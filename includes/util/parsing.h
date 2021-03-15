#ifndef PARSING_H
# define PARSING_H

# include "vector3.h"
# include "util/scene.h"

int		parse_file(t_scene *scene, char *file, int depth, t_vector3 origin);
int		col_deserialize(const char *str, t_color *color);
int		vec_deserialize(const char *str, t_vector3 *vector);
int		dir_deserialize(const char *str, t_vector3 *direction);
int		float_deserialize(char *str, float *result);
int		int_deserialize(char *str, int *result);
int		bounded_int_deserialize(char *str, int *result, int min, int max);
int		bounded_float_deserialize(char *str, float *result, float min,
			float max);
int		ulong_deserialize(char *str, unsigned long *result);
int		args_size(const char *type, int given, int expected);
t_scene	*parse(char *file);

#endif
