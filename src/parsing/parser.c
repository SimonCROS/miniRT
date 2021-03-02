#include "minirt.h"
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

t_render_params	*parse_render(t_list *data)
{
	t_render_params	params;
	t_render_params	*render_data;
	t_color		color;
	int			e;

	if (data->size != 6)
		return (NULL);
	e = 1;
	e = e && ft_atoi_full((char *)lst_get(data, 1), &params.width);
	e = e && ft_atoi_full((char *)lst_get(data, 2), &params.height);
	e = e && ft_atoi_full((char *)lst_get(data, 3), &params.threads);
	e = e && ft_atoi_full((char *)lst_get(data, 4), &params.chunk_width);
	e = e && ft_atoi_full((char *)lst_get(data, 5), &params.chunk_height);
	if (!e || params.width < 1 || params.height < 1 || params.chunk_width < 1
		|| params.chunk_height < 1 || params.threads < 1
		|| params.threads > MAX_THREADS)
		return (NULL);
	render_data = malloc(sizeof(t_render_params));
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

void	parse(t_list *line, t_scene *scene)
{
	if (ft_strcmp(lst_first(line), "R") == 0)
		scene->render = parse_render(line);
	else if (ft_strcmp(lst_first(line), "A") == 0 && !(scene->ambiant))
		scene->ambiant = parse_ambiant(line);
	else if (ft_strcmp(lst_first(line), "c") == 0)
		lst_push(scene->cameras, parse_camera(line));
	else if (ft_strcmp(lst_first(line), "l") == 0)
		lst_push(scene->lights, parse_light(line));
	else if (ft_strcmp(lst_first(line), "tr") == 0)
		lst_push(scene->objects, parse_triangle(line));
	else if (ft_strcmp(lst_first(line), "pl") == 0)
		lst_push(scene->objects, parse_plane(line));
	else if (ft_strcmp(lst_first(line), "sq") == 0)
		lst_push(scene->objects, parse_square(line));
	else if (ft_strcmp(lst_first(line), "sp") == 0)
		lst_push(scene->objects, parse_sphere(line));
	else if (ft_strcmp(lst_first(line), "cy") == 0)
		lst_push(scene->objects, parse_cylinder(line));
	// TODO errno
}

t_scene	*parse_file(char *file)
{
	t_scene *scene;
	int		fd;
	char	*buffer;
	t_list	*nodes;
	int		result;

	if (!(scene = malloc(sizeof(t_scene))))
		return (NULL);
	scene->index = 0;
	scene->ambiant = NULL;
	scene->cameras = lst_new(&free);
	scene->lights = lst_new(&free);
	scene->objects = lst_new(&free);

	fd = open(file, O_RDONLY);
	buffer = NULL;
	result = 1;
	nodes = lst_new((t_con)lst_destroy);
	while (result > 0)
	{
		result = get_next_line(fd, &buffer);
		if (result < 0)
			break ;
		if (*buffer == '#')
			continue ;
		lst_push(nodes, as_listf((void **)ft_splitf(buffer, ' '), free));
	}
	lst_filter_in(nodes, (t_pre)lst_not_empty);
	lst_foreachp(nodes, (t_bicon)parse, scene);
	lst_destroy(nodes);
	return (scene);
}
