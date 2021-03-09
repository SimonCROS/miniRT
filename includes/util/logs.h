#ifndef LOGS_H
# define LOGS_H

# include <stdio.h>

typedef enum e_log_type
{
	INFO,
	WARN,
	ERROR,
	DEBUG
}	t_log_type;

int	log_msg(t_log_type type, char *str);
int	is_debug_enabled(void);
int	set_debug(int debug);
int	log_nl(void);

#endif
