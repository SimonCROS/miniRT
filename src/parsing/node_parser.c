#include "minirt.h"
#include "object.h"

static int	duplicated_node(char *type)
{
	errno = -1;
	log_msg_arg(ERROR, "Duplicate node found (%s)", type);
	return (FALSE);
}

static int	unknown_type(char *type)
{
	errno = -1;
	log_msg_arg(ERROR, "Unknown type : \"%s\"", type);
	return (FALSE);
}

static int	parse_node3(t_list *line, t_scene *scene, int depth,
	t_vec3f origin)
{
	if (ft_strcmp(lst_first(line), "ob") == 0)
		return (parse_object(scene, line, depth, origin));
	return (unknown_type(lst_first(line)));
}

static int	parse_node2(t_list *line, t_scene *scene, int depth,
	t_vec3f origin)
{
	if (ft_strcmp(lst_first(line), "c") == 0)
		return (!!lst_unshift(scene->cameras, parse_camera(line, origin)));
	else if (ft_strcmp(lst_first(line), "l") == 0)
		return (!!lst_unshift(scene->lights, parse_light_point(line, origin)));
	else if (ft_strcmp(lst_first(line), "la") == 0)
		return (!!lst_unshift(scene->lights, parse_light_laser(line, origin)));
	else if (ft_strcmp(lst_first(line), "tr") == 0)
		return (!!lst_unshift(scene->triangles, parse_triangle(line, origin)));
	else if (ft_strcmp(lst_first(line), "pl") == 0)
		return (!!lst_unshift(scene->objects, parse_plane(line, origin)));
	else if (ft_strcmp(lst_first(line), "sq") == 0)
		return (!!lst_unshift(scene->objects, parse_square(line, origin)));
	else if (ft_strcmp(lst_first(line), "sp") == 0)
		return (!!lst_unshift(scene->objects, parse_sphere(line, origin)));
	else if (ft_strcmp(lst_first(line), "cy") == 0)
		return (!!lst_unshift(scene->objects, parse_cylinder(line, origin)));
	else if (ft_strcmp(lst_first(line), "ci") == 0)
		return (!!lst_unshift(scene->objects, parse_circle(line, origin)));
	else if (ft_strcmp(lst_first(line), "hy") == 0)
		return (!!lst_unshift(scene->objects, parse_hyperboloid(line, origin)));
	else if (ft_strcmp(lst_first(line), "cu") == 0)
		return (parse_cube(line, origin, scene->objects));
	else if (ft_strcmp(lst_first(line), "py") == 0)
		return (parse_pyramid(line, origin, scene->objects));
	return (parse_node3(line, scene, depth, origin));
}

int	parse_node(t_list *line, t_scene *scene, int depth, t_vec3f origin)
{
	if (lst_is_empty(line))
		return (1);
	if (ft_strcmp(lst_first(line), "R") == 0)
	{
		if (scene->render)
			return (duplicated_node(lst_first(line)));
		scene->render = parse_render(line);
		return (!!scene->render);
	}
	else if (ft_strcmp(lst_first(line), "A") == 0)
	{
		if (scene->ambiant)
			return (duplicated_node(lst_first(line)));
		scene->ambiant = parse_ambiant(line);
		return (!!scene->ambiant);
	}
	else if (ft_strcmp(lst_first(line), "B") == 0)
	{
		if (scene->background)
			return (duplicated_node(lst_first(line)));
		scene->background = parse_background(line);
		return (!!scene->background);
	}
	return (parse_node2(line, scene, depth, origin));
}
