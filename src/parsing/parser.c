#include "minirt.h"
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

void	*parse(t_list *line)
{
	t_object *object;

	object = NULL;
	if (ft_strcmp(lst_first(line), "tr") == 0)
		object = parse_triangle(line);
	else if (ft_strcmp(lst_first(line), "pl") == 0)
		object = parse_plane(line);
	else if (ft_strcmp(lst_first(line), "sq") == 0)
		object = parse_square(line);
	else if (ft_strcmp(lst_first(line), "sp") == 0)
		object = parse_sphere(line);
	else if (ft_strcmp(lst_first(line), "cy") == 0)
		object = parse_cylinder(line);
	else
		exit(0);
	lst_destroy(line);
	return (object);
}

t_list	*parse_file(char *file)
{
	int		fd;
	char	*buffer;
	t_list	*nodes;
	int		result; 

	fd = open(file, O_RDONLY);
	buffer = NULL;
	result = 1;
	nodes = lst_new((t_con)lst_destroy);
	while (result > 0)
	{
		result = get_next_line(fd, &buffer);
		if (result < 0)
			break ;
		if (*buffer == '#')
			continue ;
		lst_push(nodes, as_listf((void **)ft_splitf(buffer, ' '), free));
	}
	lst_filter_in(nodes, (t_pre)lst_not_empty);
	lst_map_in(nodes, (t_fun)parse, free);
	return (nodes);
}
