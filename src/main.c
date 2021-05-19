#include "minirt.h"

int	main(int argc, char **argv)
{
	t_scene	*scene;
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
	scene = parse(argv[1]);
	if (!scene)
	{
		log_msg(FATAL, "An error occurred while parsing the file");
		perror("Error\nAn error occurred while parsing the file");
		exit(EXIT_FAILURE);
	}
	log_msg(INFO, "Parsing finished");
	if (save)
		init_bmp_image(argv[1], scene);
	else
		init_window(argv[1], scene);
	exit(EXIT_SUCCESS);
}
