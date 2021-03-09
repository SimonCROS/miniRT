#include <pthread.h>

#include "util/logs.h"

static int	provider(int debug)
{
	static int	saved_debug;

	if (debug != -1)
		saved_debug = debug;
	return (saved_debug);
}

int	is_debug_enabled(void)
{
	return (provider(-1));
}

int	set_debug(int debug)
{
	return (provider(!!debug));
}

int	log_nl(void)
{
	return (printf("\033[0m\n"));
}

int	log_msg(t_log_type type, char *str)
{
	char	thread[50];
	int		len;

	pthread_getname_np(pthread_self(), thread, 50);
	if (type == INFO)
		len = printf("\033[0m[%s] [INFO]: ", thread);
	else if (type == WARN)
		len = printf("\033[33m[%s] [WARN]: ", thread);
	else if (type == ERROR)
		len = printf("\033[31m[%s] [ERROR]: ", thread);
	else if (type == DEBUG && is_debug_enabled())
		len = printf("\033[90m[%s] [DEBUG]: ", thread);
	else
		return (0);
	if (str)
		len += printf("%s\033[0m\n", str);
	return (len);
}
