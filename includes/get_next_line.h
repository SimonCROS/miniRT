/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 13:45:04 by scros             #+#    #+#             */
/*   Updated: 2021/02/26 11:04:53 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# define BUFFER_SIZE 50

typedef struct			s_gnllist
{
	int					fd;
	char				*content;
	struct s_gnllist	*next;
}						t_gnllist;

size_t					ft_strlen(const char *s);
void					*ft_memcpy(void *dst, const void *src, size_t n);
char					**ft_split_first(char *s, char c);
char					*ft_substr(const char *s, unsigned int start, size_t len);
char					*ft_strjoin(const char *s1, const char *s2);
char					*ft_strchr(const char *s, int c);
int						get_next_line(int fd, char **line);

#endif
