#include "minirt.h"

void	*free_scene(t_scene *scene)
{
	if (!scene)
		return (NULL);
	free(scene->render);
	free(scene->ambiant);
	lst_destroy(scene->cameras);
	lst_destroy(scene->lights);
	lst_destroy(scene->objects);
	free(scene->background);
	free(scene);
	return (NULL);
}
