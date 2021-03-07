#include "util/get_next_line.h"

int		delete(int ret, t_gnllist **remain, t_gnllist *element, void *p)
{
	t_gnllist *elem;

	if (remain && *remain && element)
	{
		if (element == *remain)
			*remain = element->next;
		else if ((elem = *remain))
			while (elem)
			{
				if (elem->next == element)
				{
					elem->next = element->next;
					break ;
				}
				elem = elem->next;
			}
		if (element->content)
			free(element->content);
		free(element);
	}
	if (p)
		free(p);
	return (ret);
}

t_gnllist	*get_or_create_remain(t_gnllist **remain, int fd)
{
	t_gnllist *new_element;
	t_gnllist *elem;

	elem = *remain;
	while (elem)
	{
		if (elem->fd == fd)
			return (elem);
		elem = elem->next;
	}
	if (!(new_element = malloc(sizeof(*new_element))))
		return (NULL);
	new_element->content = NULL;
	new_element->fd = fd;
	new_element->next = *remain;
	*remain = new_element;
	return (new_element);
}

ssize_t	read_line(int fd, char **line, char ***current, t_gnllist *remain)
{
	char	*tmp;
	ssize_t	result;

	if (!(remain->content = malloc(BUFFER_SIZE + 1)))
		return (-1);
	result = read(fd, remain->content, BUFFER_SIZE);
	if (result < 0)
		return (-1);
	if (*current)
		free(*current);
	if (result >= 0)
	{
		remain->content[result] = 0;
		if (!(*current = ft_split_first(remain->content, '\n')))
			return (-1);
		if (result > 0)
		{
			tmp = *line;
			if (!(*line = ft_strjoin(tmp, (*current)[0])))
				return (-1);
			free(tmp);
		}
		free((*current)[0]);
	}
	return (result);
}

int		load_remain(t_gnllist *remain, char **line, char ***current)
{
	if (remain->content)
	{
		if (!(*current = ft_split_first(remain->content, '\n')))
			return (-1);
		free(*line);
		*line = (*current)[0];
	}
	return (1);
}

int		get_next_line(int fd, char **line)
{
	static t_gnllist	*remain_lst;
	char				**current;
	char				*tmp_line;
	t_gnllist			*remain;
	ssize_t				result;

	if (BUFFER_SIZE < 1 || !(tmp_line = malloc(0)))
		return (-1);
	*tmp_line = 0;
	if (!(remain = get_or_create_remain(&remain_lst, fd)))
		return (delete(-1, NULL, NULL, tmp_line));
	current = NULL;
	if (load_remain(remain, &tmp_line, &current) == -1)
		return (delete(-1, &remain_lst, remain, tmp_line));
	result = BUFFER_SIZE;
	while (result == BUFFER_SIZE && (!current || !current[1]))
		if ((result = read_line(fd, &tmp_line, &current, remain)) == -1)
			return (delete(-1, &remain_lst, remain, tmp_line));
	remain->content = current[1];
	free(current);
	*line = tmp_line;
	if (result == BUFFER_SIZE || (result && remain->content))
		return (1);
	return (delete(0, &remain_lst, remain, NULL));
}
