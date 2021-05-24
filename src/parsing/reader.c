#include <fcntl.h>

#include "minirt.h"
#include "object.h"

static t_read_status	read_line(t_list *nds, int fd)
{
	int		result;
	char	*buffer;

	buffer = NULL;
	result = get_next_line(fd, &buffer);
	if (result < 0)
		return (READ_ERROR);
	if (*buffer == '#')
		free(buffer);
	else if (!lst_unshift(nds, as_listf((void **)ft_splitf(buffer, ' '), free)))
	{
		errno = -1;
		return (READ_ERROR);
	}
	if (result == 0)
		return (READ_EOF);
	return (READ_SUCCESS);
}

static int	read_lines(t_list *nodes, char *file, int fd)
{
	t_read_status	result;
	int				reading;

	result = READ_SUCCESS;
	reading = 0;
	while (result == READ_SUCCESS)
	{
		if (!(reading % 1000))
		{
			log_msg_arg(INFO, "\033[33m< Reading\033[0m %s...", file);
			log_prev_line();
		}
		reading++;
		result = read_line(nodes, fd);
	}
	return (result != READ_ERROR);
}

static int	validate(char *file, int depth)
{
	if (ft_strlen(file) < 4 || !ft_ends_with(file, ".rt"))
	{
		errno = -1;
		log_msg(ERROR, "File is not a \".rt\" file.");
		return (FALSE);
	}
	if (depth == 5)
	{
		errno = EWOULDBLOCK;
		log_msg(ERROR,
			"Maximum file depth reached, maybe you have a circular inclusion.");
		return (FALSE);
	}
	return (TRUE);
}

int	read_file(char *file, int depth, t_list **nodes)
{
	int	fd;

	if (!validate(file, depth))
		return (FALSE);
	*nodes = lst_new((t_con)lst_destroy);
	if (!*nodes)
		return (FALSE);
	fd = open(file, O_RDONLY);
	if (fd < 0 || !read_lines(*nodes, file, fd))
	{
		lst_destroy(*nodes);
		close(fd);
		return (FALSE);
	}
	close(fd);
	return (TRUE);
}
