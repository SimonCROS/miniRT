#include "minirt.h"

static char	**ft_split_first_next(char **parts, char *s)
{
	size_t	len;

	len = 0;
	if (!parts[1])
		len = -1;
	else
		len += parts[1] - s - 1;
	parts[0] = ft_substr(s, 0, len);
	if (!parts[0])
		return (NULL);
	if (parts[1])
	{
		parts[1] = ft_substr(parts[1], 0, -1);
		if (!parts[1])
			return (NULL);
	}
	free(s);
	return (parts);
}

char	**ft_split_first(char *s, char c)
{
	char	**parts;
	size_t	i;

	parts = malloc(sizeof(*parts) * 2);
	if (!parts)
		return (NULL);
	i = 0;
	parts[1] = NULL;
	while (s[i] && s[i++] != c)
		;
	if (i == 0)
		i++;
	if (s[i - 1] == c)
		parts[1] = s + i;
	return (ft_split_first_next(parts, s));
}
