#include "minirt.h"

int	main(int argc, char **argv)
{
	int		save;

	pthread_setname_ft("MAIN");
	log_msg(INFO, "Starting program...");
	save = 0;
	if (argc == 3 && !ft_strcmp(argv[2], "--save"))
		save = 1;
	else if (argc != 2)
	{
		errno = EINVAL;
		perror("Error\nUnable to parse program arguments");
		exit(EXIT_FAILURE);
	}
	if (!load_scene(argv[1]))
	{
		log_msg(FATAL, "An error occurred while parsing the file");
		perror("Error\nAn error occurred while parsing the file");
		exit(EXIT_FAILURE);
	}
	log_msg(INFO, "Parsing finished");
	if (save)
		init_bmp_image(argv[1], get_scene());
	else
		init_window(argv[1], get_scene());
	exit(EXIT_SUCCESS);
}
