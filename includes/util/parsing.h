#ifndef PARSING_H
# define PARSING_H

# include "vector3.h"
# include "util/scene.h"

int		parse_file(t_scene *scene, char *file, int depth, t_vector3 origin);
t_scene	*parse(char *file);

#endif
