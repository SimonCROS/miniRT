#ifndef LOGS_H
# define LOGS_H

# include <stdio.h>

void	log_nl(void);
int		log_info(char *str);
int		log_warn(char *str);
int		log_error(char *str);
int		log_debug(char *str);

#endif
