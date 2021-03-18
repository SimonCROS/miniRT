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

	if (!args_size(lst_first(data), data->size, 6))
		return (NULL);
	if (!ulong_deserialize((char *)lst_get(data, 1), &params.width)
		|| !ulong_deserialize((char *)lst_get(data, 2), &params.height)
		|| !bounded_int_deserialize((char *)lst_get(data, 3), &params.threads,
			1, MAX_THREADS)
		|| !ulong_deserialize((char *)lst_get(data, 4), &params.chunk_width)
		|| !ulong_deserialize((char *)lst_get(data, 5), &params.chunk_height))
		return (NULL);
	render_data = malloc(sizeof(t_options));
	if (render_data)
		*render_data = params;
	return (render_data);
}

t_color	*parse_background(t_list *data)
{
	t_color		background;

	if (!args_size(lst_first(data), data->size, 2))
		return (NULL);
	if (!col_deserialize((char *)lst_get(data, 1), &background))
		return (NULL);
	return (color_clone(background));
}

t_color	*parse_ambiant(t_list *data)
{
	float		brightness;
	t_color		color;

	if (!args_size(lst_first(data), data->size, 3))
		return (NULL);
	if (!bounded_float_deserialize((char *)lst_get(data, 1), &brightness, 0,
			1)
		|| !col_deserialize((char *)lst_get(data, 2), &color))
		return (NULL);
	return (color_clone(color_mulf(color, brightness)));
}

int	parse_object(t_scene *scene, t_list *data, int depth, t_vector3 origin)
{
	t_vector3	pos;
	char		*file;

	if (!args_size(lst_first(data), data->size, 3))
		return (FALSE);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos))
		return (FALSE);
	file = ft_strtrim((char *)lst_get(data, 2), "\"");
	if (!parse_file(scene, file, depth, vec3_addv(pos, origin)))
	{
		free(file);
		return (FALSE);
	}
	free(file);
	return (TRUE);
}

int	parse_node(t_list *line, t_scene *scene, int depth, t_vector3 origin)
{
	void	*node;

	if (ft_strcmp(lst_first(line), "R") == 0)
	{
		if (scene->render)
		{
			errno = -1;
			log_msg(ERROR, "Found duplicated render node (R)");
			return (FALSE);
		}
		scene->render = parse_render(line);
		node = scene->render;
	}
	else if (ft_strcmp(lst_first(line), "A") == 0)
	{
		if (scene->ambiant)
		{
			errno = -1;
			log_msg(ERROR, "Found duplicated ambiant node (A)");
			return (FALSE);
		}
		scene->ambiant = parse_ambiant(line);
		node = scene->ambiant;
	}
	else if (ft_strcmp(lst_first(line), "B") == 0)
	{
		if (scene->background)
		{
			errno = -1;
			log_msg(ERROR, "Found duplicated background node (B)");
			return (FALSE);
		}
		scene->background = parse_background(line);
		node = scene->background;
	}
	else if (ft_strcmp(lst_first(line), "c") == 0)
		node = lst_push(scene->cameras, parse_camera(line, origin));
	else if (ft_strcmp(lst_first(line), "l") == 0)
		node = lst_push(scene->lights, parse_light(line, origin));
	else if (ft_strcmp(lst_first(line), "tr") == 0)
		node = lst_push(scene->objects, parse_triangle(line, origin));
	else if (ft_strcmp(lst_first(line), "pl") == 0)
		node = lst_push(scene->objects, parse_plane(line, origin));
	else if (ft_strcmp(lst_first(line), "sq") == 0)
		node = lst_push(scene->objects, parse_square(line, origin));
	else if (ft_strcmp(lst_first(line), "sp") == 0)
		node = lst_push(scene->objects, parse_sphere(line, origin));
	else if (ft_strcmp(lst_first(line), "cy") == 0
		|| ft_strcmp(lst_first(line), "ccy") == 0)
		node = lst_push(scene->objects, parse_cylinder(line, origin));
	else if (ft_strcmp(lst_first(line), "ci") == 0)
		node = lst_push(scene->objects, parse_circle(line, origin));
	else if (ft_strcmp(lst_first(line), "ob") == 0)
		return (parse_object(scene, line, depth, origin));
	else
	{
		errno = -1;
		log_msg(ERROR, NULL);
		printf("Unknown type : \"%s\"", (char *)lst_first(line));
		log_nl();
		return (FALSE);
	}
	return (!!node);
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
		{
			free(buffer);
			continue ;
		}
		lst_push(nodes, as_listf((void **)ft_splitf(buffer, ' '), free));
	}
	return (result != -1);
}

int	parse_file(t_scene *scene, char *file, int depth, t_vector3 origin)
{
	t_list		*nodes;
	t_iterator	iterator;
	int			success;
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
	{
		lst_destroy(nodes);
		return (FALSE);
	}
	close(fd);
	lst_filter_in(nodes, (t_pre)lst_not_empty);
	iterator = iterator_new(nodes);
	success = 1;
	while (success && iterator_has_next(&iterator))
		success = parse_node(iterator_next(&iterator), scene, depth, origin);
	lst_destroy(nodes);
	return (success);
}

int	is_scene_valid(t_scene *scene)
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
	if (!scene->cameras || !scene->lights || !scene->objects
		|| !parse_file(scene, file, 0, vec3_new(0, 0, 0))
		|| !is_scene_valid(scene))
		return (free_scene(scene));
	return (scene);
}
