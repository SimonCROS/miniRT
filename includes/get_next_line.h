/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: praclet <praclet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 22:21:11 by praclet           #+#    #+#             */
/*   Updated: 2020/12/13 10:01:25 by praclet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// TODO replace by my gnl

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>

# define BUFFER_SIZE 50

typedef struct		s_file
{
	int				fd;
	int				state;
	int				start;
	int				end;
	int				pos;
	char			buffer[BUFFER_SIZE];
}					t_file;

typedef struct		s_gnllist
{
	void			*data;
	struct s_gnllist	*next;
}					t_gnllist;

int					get_next_line(int fd, char **line);
t_file				*gnllst_get(t_gnllist **list, int fd);
char				*gnl_concat(char *s1, char *s2, size_t len2);
void				gnllst_remove(t_gnllist **list, t_file **file);

#endif
