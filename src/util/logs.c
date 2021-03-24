#include "minirt.h"

int	log_nl(void)
{
	return (printf("\033[0m\n"));
}

int	log_msg(t_log_type type, char *str)
{
	char	thread[50];
	int		len;

	pthread_getname_ft(thread, 50);
	if (type == INFO)
		len = printf("\033[0m[%s/INFO]: ", thread);
	else if (type == WARN)
		len = printf("\033[33m[%s/WARN]: ", thread);
	else if (type == ERROR)
		len = printf("\033[1;31m[%s/ERROR]: ", thread);
	else if (type == FATAL)
		len = printf("\033[31m[%s/FATAL]: ", thread);
	else if (type == DEBUG && is_debug_enabled())
		len = printf("\033[90m[%s/DEBUG]: ", thread);
	else
		return (0);
	if (str)
		len += printf("%s\033[0m\n", str);
	return (len);
}

int	log_msg_arg(t_log_type type, char *str, const char *arg)
{
	char	thread[50];
	int		len;

	pthread_getname_ft(thread, 50);
	if (type == INFO)
		len = printf("\033[0m[%s/INFO]: ", thread);
	else if (type == WARN)
		len = printf("\033[33m[%s/WARN]: ", thread);
	else if (type == ERROR)
		len = printf("\033[1;31m[%s/ERROR]: ", thread);
	else if (type == FATAL)
		len = printf("\033[31m[%s/FATAL]: ", thread);
	else if (type == DEBUG && is_debug_enabled())
		len = printf("\033[90m[%s/DEBUG]: ", thread);
	else
		return (0);
	if (str)
		len += printf(str, arg);
	log_nl();
	return (len);
}
