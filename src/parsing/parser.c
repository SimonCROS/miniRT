#include <fcntl.h>

#include "minirt.h"
#include "object.h"

static int	parse_lines(t_list *nodes, int fd)
{
	int		result;
	char	*buffer;
	int		reading;

	buffer = NULL;
	result = 1;
	reading = 0;
	while (result > 0)
	{
		reading++;
		printf("Reading lines... %d\r", reading);
		result = get_next_line(fd, &buffer);
		if (result < 0)
			break ;
		if (*buffer == '#')
		{
			free(buffer);
			continue ;
		}
		if (!lst_unshift(nodes, as_listf((void **)ft_splitf(buffer, ' '), free)))
		{
			errno = -1;
			return (0);
		}
	}
	printf("Reading lines... %d\n", reading);
	return (result != -1);
}

static int	max_depth_file(void)
{
	errno = EWOULDBLOCK;
	log_msg(ERROR,
		"Maximum file depth reached, maybe you have a circular inclusion.");
	return (FALSE);
}

int	parse_file(t_scene *scene, char *file, int depth, t_vector3 origin)
{
	t_list		*nodes;
	t_iterator	iterator;
	int			success;
	int			fd;
	int			parsing;

	if (++depth == 5)
		return (max_depth_file());
	nodes = lst_new((t_con)lst_destroy);
	if (!nodes)
		return (FALSE);
	fd = open(file, O_RDONLY);
	if (fd < 0 || !parse_lines(nodes, fd))
	{
		lst_destroy(nodes);
		close(fd);
		return (FALSE);
	}
	close(fd);
	iterator = iterator_new(nodes);
	success = 1;
	parsing = 0;
	while (success && iterator_has_next(&iterator))
	{
		parsing++;
		printf("Parsing lines... %d\r", parsing);
		success = parse_node(iterator_next(&iterator), scene, depth, origin);
	}
	printf("Parsing lines... %d\n", parsing);
	lst_destroy(nodes);
	return (success);
}

static int	is_scene_valid(t_scene *scene)
{
	if (!scene->render || !scene->ambiant || !scene->cameras->size)
	{
		errno = -1;
		if (!scene->render)
			log_msg(ERROR, "Render node (R) not defined.");
		if (!scene->ambiant)
			log_msg(ERROR, "Ambiant node (A) not defined.");
		if (!scene->cameras->size)
			log_msg(ERROR, "No camera defined.");
		return (FALSE);
	}
	if (!scene->background)
	{
		log_msg(WARN, "Background node (B) not defined (default to black).");
		scene->background = color_clone(color_new(0, 0, 0));
		if (!scene->background)
			return (FALSE);
	}
	return (TRUE);
}

t_scene	*parse(char *file)
{
	t_scene	*scene;

	if (!file)
		return (NULL);
	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	setbuf(stdout, NULL);
	scene->index = 0;
	scene->render = NULL;
	scene->ambiant = NULL;
	scene->background = NULL;
	scene->cameras = lst_new(&free);
	scene->lights = lst_new(&free);
	scene->objects = lst_new(&free);
	scene->triangles = lst_new(&free);
	if (!scene->cameras || !scene->lights || !scene->objects
		|| !parse_file(scene, file, 0, vec3_new(0, 0, 0))
		|| !is_scene_valid(scene))
		return (free_scene(scene));
	return (scene);
}
