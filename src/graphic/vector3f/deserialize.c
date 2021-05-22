#include "libft.h"
#include "graphics.h"

int	vec3_deserialize(const char *str, t_vec3f *vector)
{
	t_list	*parts;
	t_vec3f	result;
	int		ret;

	if (ft_strcnt(str, ',') != 2)
		return (0);
	parts = as_listf((void **)ft_split(str, ','), &free);
	if (!parts)
		return (0);
	ret = 0;
	if (parts->size == 3)
	{
		if (ft_atof_full(lst_get(parts, 0), &(result.x))
			&& ft_atof_full(lst_get(parts, 1), &(result.y))
			&& ft_atof_full(lst_get(parts, 2), &(result.z)))
		{
			ret = 1;
			*vector = result;
		}
	}
	lst_destroy(parts);
	return (ret);
}
