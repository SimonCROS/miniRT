#include "minirt.h"
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

void	print_part(void *str)
{
	printf("%s ", (char *)str);
}

void	*parse(t_list *line)
{
	lst_foreach(line, print_part);
	printf("\n");
	lst_destroy(line);
	return (NULL);
}

int		lst_not_empty(t_list *list)
{
	return (!lst_is_empty(list));
}

t_scene	*parse_file(char *file)
{
	int		fd;
	char	*buffer;
	t_list	*nodes;
	int		result; 

	fd = open(file, O_RDONLY);
	buffer = NULL;
	result = 1;
	nodes = lst_new((t_consumer)lst_destroy);
	while (result > 0)
	{
		result = get_next_line(fd, &buffer);
		if (result < 0)
			break;
		lst_push(nodes, as_listf((void **)ft_splitf(buffer, ' '), free));
	}
	lst_filter_in(nodes, (t_pre)lst_not_empty);
	lst_map_in(nodes, (t_fun)parse, free);
	lst_destroy(nodes);
	return (NULL);
}
