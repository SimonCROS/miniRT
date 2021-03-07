#include "provider/scene_provider.h"
#include <stdlib.h>

static t_scene	*provider(char *file)
{
	static t_scene	*scene;

	if (file)
		scene = parse_file(file);
	return (scene);
}

t_scene	*get_scene(void)
{
	return (provider(NULL));
}

int	set_scene(char *file)
{
	return (!!provider(file));
}