#ifndef OPTIONS_H
# define OPTIONS_H

typedef struct s_options {
	size_t			width;
	size_t			height;
	int				threads;
	size_t			chunk_width;
	size_t			chunk_height;
}	t_options;

#endif
