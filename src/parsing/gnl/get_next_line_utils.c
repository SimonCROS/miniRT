#include "minirt.h"

char	**ft_split_first(char *s, char c)
{
	char	**parts;
	int		i;

	parts = malloc(sizeof(*parts) * 2);
	if (!parts)
		return (NULL);
	i = ft_strindex_of(s, c);
	parts[1] = NULL;
	if (i == -1)
		parts[0] = s;
	else
	{
		parts[0] = ft_substr(s, 0, i);
		parts[1] = ft_strdup(s + i + 1);
		free(s);
		if (!parts[0] || !parts[1])
		{
			free(parts[0]);
			free(parts[1]);
			free(parts);
			return (NULL);
		}
	}
	return (parts);
}
