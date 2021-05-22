#include "minirt.h"

void	start_impl(char *file, t_scene *scene, t_impl impl)
{
	if (impl == USE_BMP)
		init_bmp_image(file, scene);
	else
		init_window(file, scene);
}

int	main(int argc, char **argv)
{
	t_scene	*scene;
	t_impl	impl;

	pthread_setname_ft("MAIN");
	log_msg(INFO, "Starting program...");
	impl = USE_MLX;
	if (argc == 3 && !ft_strcmp(argv[2], "--save"))
		impl = USE_BMP;
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
	start_impl(argv[1], scene, impl);
	exit(EXIT_SUCCESS);
}
