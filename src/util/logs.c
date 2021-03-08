#include <pthread.h>

#include "util/logs.h"

void	log_nl(void)
{
	printf("\033[0m\n");
}

int	log_info(char *str)
{
	char	thread[50];
	int		len;

	pthread_getname_np(pthread_self(), thread, 50);
	if (str)
		len = printf("\033[0m[%s] [INFO]: %s\033[0m\n", thread, str);
	else
		len = printf("\033[0m[%s] [INFO]: ", thread);
	return (len);
}

int	log_warn(char *str)
{
	char	thread[50];
	int		len;

	pthread_getname_np(pthread_self(), thread, 50);
	if (str)
		len = printf("\033[33m[%s] [WARN]: %s\033[0m\n", thread, str);
	else
		len = printf("\033[33m[%s] [WARN]: ", thread);
	return (len);
}

int	log_error(char *str)
{
	char	thread[50];
	int		len;

	pthread_getname_np(pthread_self(), thread, 50);
	if (str)
		len = printf("\033[31m[%s] [ERROR]: %s\033[0m\n", thread, str);
	else
		len = printf("\033[31m[%s] [ERROR]: ", thread);
	return (len);
}

int	log_debug(char *str)
{
	char	thread[50];
	int		len;

	pthread_getname_np(pthread_self(), thread, 50);
	if (str)
		len = printf("\033[90m[%s] [DEBUG]: %s\033[0m\n", thread, str);
	else
		len = printf("\033[90m[%s] [DEBUG]: ", thread);
	return (len);
}
