#include "minirt.h"
#include "object.h"

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
	printf("\n");
	if (!parse_file(scene, file, depth, vec3_addv(pos, origin)))
	{
		free(file);
		return (FALSE);
	}
	free(file);
	return (TRUE);
}
