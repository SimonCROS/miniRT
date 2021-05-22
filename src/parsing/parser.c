#include "minirt.h"
#include "object.h"

static void	parse_log(char *file, int parsing, int size)
{
	if (!(parsing % (int)(size / 100 + 1)))
	{
		log_msg(INFO, NULL);
		printf("\33[32m> Parsing\33[0m %s... (\33[33m%d\33[0m) \33[32m%d%%",
			file, parsing, parsing * 100 / size);
		log_nl();
		log_prev_line();
	}
}

int	parse_file(t_scene *scene, char *file, int depth, t_vec3f origin)
{
	t_list		*nodes;
	t_iterator	iterator;
	int			success;
	int			parsing;

	if (!read_file(file, depth, &nodes))
		return (FALSE);
	iterator = iterator_new(nodes);
	success = 1;
	parsing = 0;
	while (success && iterator_has_next(&iterator))
	{
		parse_log(file, parsing, nodes->size);
		parsing++;
		success = parse_node(iterator_next(&iterator), scene, depth, origin);
	}
	log_msg(INFO, NULL);
	printf("\33[32m> Parsing\33[0m %s... (\33[33m%d\33[0m) \33[32m%d%%",
		file, parsing, parsing * 100 / nodes->size);
	log_nl();
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
