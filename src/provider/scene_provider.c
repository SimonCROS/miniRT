#include "minirt.h"

static t_scene	*provider(t_scene *scene)
{
	static t_scene	*saved_scene;

	if (scene)
		saved_scene = scene;
	return (saved_scene);
}

t_scene	*get_scene(void)
{
	return (provider(NULL));
}

int	load_scene(char *file)
{
	return (!!provider(parse(file)));
}
