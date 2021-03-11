#include <fcntl.h>

#include "list.h"
#include "color.h"
#include "convert.h"

#include "minirt.h"
#include "options.h"
#include "element/camera.h"
#include "element/light.h"
#include "element/plan.h"
#include "util/get_next_line.h"
#include "util/scene.h"
#include "util/logs.h"
#include "util/parsing.h"

t_options	*parse_render(t_list *data)
{
	t_options	params;
	t_options	*render_data;
	int			e;

	if (data->size != 6)
		return (NULL);
	e = 1;
	e = e && ft_atoul_full((char *)lst_get(data, 1), &params.width);
	e = e && ft_atoul_full((char *)lst_get(data, 2), &params.height);
	e = e && ft_atoi_full((char *)lst_get(data, 3), &params.threads);
	e = e && ft_atoul_full((char *)lst_get(data, 4), &params.chunk_width);
	e = e && ft_atoul_full((char *)lst_get(data, 5), &params.chunk_height);
	if (!e || params.threads < 1 || params.threads > MAX_THREADS)
		return (NULL);
	render_data = malloc(sizeof(t_options));
	if (render_data)
		*render_data = params;
	return (render_data);
}

t_color	*parse_ambiant(t_list *data)
{
	float		brightness;
	t_color		color;
	int			e;

	if (data->size != 3)
		return (NULL);
	e = 1;
	e = e && ft_atof_full((char *)lst_get(data, 1), &brightness);
	e = e && color_deserialize((char *)lst_get(data, 2), &color);
	if (!e || brightness < 0 || brightness > 1)
		return (NULL);
	return (color_clone(color_mulf(color, brightness)));
}

int	parse_object(t_scene *scene, t_list *data, int depth, t_vector3 origin)
{
	t_vector3	pos;
	char		*file;
	int			e;

	if (data->size != 3)
		return (FALSE);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	if (!e)
		return (FALSE);
	file = ft_strtrim((char *)lst_get(data, 2), "\"");
	if (!parse_file(scene, file, depth, vec3_addv(pos, origin)))
		return (FALSE);
	free(file);
	return (TRUE);
}

void	parse_node(t_list *line, t_scene *scene, int depth, t_vector3 origin)
{
	if (ft_strcmp(lst_first(line), "R") == 0)
	{
		if (scene->render)
		{
			errno = -1;
			log_msg(ERROR, "Found duplicated render node (R), skipping...");
		}
		else
			scene->render = parse_render(line);
	}
	else if (ft_strcmp(lst_first(line), "A") == 0 && !(scene->ambiant))
	{
		if (scene->render)
		{
			errno = -1;
			log_msg(ERROR, "Found duplicated ambiant node (A), skipping...");
		}
		else
			scene->ambiant = parse_ambiant(line);
	}
	else if (ft_strcmp(lst_first(line), "c") == 0)
		lst_push(scene->cameras, parse_camera(line, origin));
	else if (ft_strcmp(lst_first(line), "l") == 0)
		lst_push(scene->lights, parse_light(line, origin));
	else if (ft_strcmp(lst_first(line), "tr") == 0)
		lst_push(scene->objects, parse_triangle(line, origin));
	else if (ft_strcmp(lst_first(line), "pl") == 0)
		lst_push(scene->objects, parse_plane(line, origin));
	else if (ft_strcmp(lst_first(line), "sq") == 0)
		lst_push(scene->objects, parse_square(line, origin));
	else if (ft_strcmp(lst_first(line), "sp") == 0)
		lst_push(scene->objects, parse_sphere(line, origin));
	else if (ft_strcmp(lst_first(line), "cy") == 0)
		lst_push(scene->objects, parse_cylinder(line, origin));
	else if (ft_strcmp(lst_first(line), "ci") == 0)
		lst_push(scene->objects, parse_circle(line, origin));
	else if (ft_strcmp(lst_first(line), "ob") == 0)
		parse_object(scene, line, depth, origin);
	else
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unknown type : %s", (char *)lst_first(line));
		log_nl();
	}
}

int	parse_lines(t_list *nodes, int fd)
{
	int		result;
	char	*buffer;

	buffer = NULL;
	result = 1;
	while (result > 0)
	{
		result = get_next_line(fd, &buffer);
		if (result < 0)
			break ;
		if (*buffer == '#')
			continue ;
		lst_push(nodes, as_listf((void **)ft_splitf(buffer, ' '), free));
	}
	return (result != -1);
}

int	parse_file(t_scene *scene, char *file, int depth, t_vector3 origin)
{
	t_list		*nodes;
	t_iterator	iterator;
	int			fd;

	if (++depth == 5)
	{
		errno = EWOULDBLOCK;
		log_msg(ERROR,
			"Maximum file depth reached, maybe you have a circular inclusion.");
		return (FALSE);
	}
	nodes = lst_new((t_con)lst_destroy);
	if (!nodes)
		return (FALSE);
	fd = open(file, O_RDONLY);
	if (fd < 0 || !parse_lines(nodes, fd))
		return (FALSE);
	lst_filter_in(nodes, (t_pre)lst_not_empty);
	iterator = iterator_new(nodes);
	while (!errno && iterator_has_next(&iterator))
		parse_node(iterator_next(&iterator), scene, depth, origin);
	lst_destroy(nodes);
	return (!errno && TRUE);
}

t_scene	*parse(char *file)
{
	t_scene	*scene;

	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	scene->index = 0;
	scene->render = NULL;
	scene->ambiant = NULL;
	scene->cameras = lst_new(&free);
	scene->lights = lst_new(&free);
	scene->objects = lst_new(&free);
	if (!parse_file(scene, file, 0, vec3_new(0, 0, 0)))
	{
		// TODO free scenes
		return (NULL);
	}
	return (scene);
}
