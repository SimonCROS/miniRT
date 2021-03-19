#ifndef PARSING_H
# define PARSING_H

# include "libft.h"

# include "util/scene.h"

int			parse_file(t_scene *scene, char *file, int depth, t_vector3 origin);
int			col_deserialize(const char *str, t_color *color);
int			vec_deserialize(const char *str, t_vector3 *vector);
int			dir_deserialize(const char *str, t_vector3 *direction);
int			float_deserialize(char *str, float *result);
int			int_deserialize(char *str, int *result);
int			bounded_int_deserialize(char *str, int *result, int min, int max);
int			bounded_float_deserialize(char *str, float *result, float min,
				float max);
int			ulong_deserialize(char *str, unsigned long *result);
int			args_size(const char *type, int given, int expected);
int			parse_object(t_scene *sce, t_list *data, int depth, t_vector3 ori);
int			parse_node(t_list *line, t_scene *scene, int depth, t_vector3 ori);
t_options	*parse_render(t_list *data);
t_color		*parse_ambiant(t_list *data);
t_color		*parse_background(t_list *data);
t_scene		*parse(char *file);

#endif
