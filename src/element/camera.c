#include "minirt.h"
#include "math.h"

t_camera	*parse_camera(t_list *data, t_vector3 origin)
{
	t_vector3	pos;
	t_vector3	rot;
	int			fov;

	if (!args_size(lst_first(data), data->size, 4))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 1), &pos)
		|| !dir_deserialize((char *)lst_get(data, 2), &rot)
		|| !bounded_int_deserialize((char *)lst_get(data, 3), &fov, 0, 180))
		return (NULL);
	return (new_camera(vec3_addv(pos, origin), rot, fov));
}

static t_matrix44	look_at(t_vector3 from, t_vector3 to)
{
	static t_vector3	tmp = (t_vector3){0, 1, 0};
	t_matrix44			c2w;
	t_vector3			forward;
	t_vector3			right;
	t_vector3			up;

	c2w = mat44_null();
	forward = vec3_normalize(vec3_subv(from, to));
	right = vec3_normalize(vec3_crossv(tmp, forward));
	if (vec3_length_squared(right) == 0)
		right = vec3_new(1, 0, 0);
	up = vec3_crossv(forward, right);
	if (vec3_length_squared(up) == 0)
		up = vec3_new(0, 1, 0);
	ft_memcpy(c2w.e[0], &right, sizeof(t_vector3));
	ft_memcpy(c2w.e[1], &up, sizeof(t_vector3));
	ft_memcpy(c2w.e[2], &forward, sizeof(t_vector3));
	ft_memcpy(c2w.e[3], &from, sizeof(t_vector3));
	c2w.e[3][3] = 1;
	return (c2w);
}

void	reload_camera(t_camera *camera)
{
	static t_vector3	up = (t_vector3){0, 1, 0};

	camera->flat = camera->direction;
	camera->flat.y = 0;
	camera->flat = vec3_normalize(camera->flat);
	if (!vec3_length_squared(camera->flat))
		camera->flat = vec3_new(0, 0, 1);
	camera->right = vec3_normalize(vec3_crossv(camera->direction, up));
	if (!vec3_length_squared(camera->right))
		camera->right = vec3_new(1, 0, 0);
	camera->up = vec3_normalize(vec3_crossv(camera->direction, camera->right));
	if (!vec3_length_squared(camera->up))
		camera->up = vec3_new(1, 0, 0);
	camera->direction = vec3_normalize(camera->direction);
	camera->c2w = look_at(vec3_new(0, 0, 0), camera->direction);
	camera->w2c = mat44_inverse(camera->c2w);
}

t_camera	*new_camera(t_vector3 position, t_vector3 direction, float fov)
{
	t_camera	*camera;

	camera = ft_calloc(1, sizeof(t_camera));
	if (!camera)
		return (NULL);
	camera->position = position;
	camera->direction = direction;
	reload_camera(camera);
	camera->hlen = tan(fov * 0.5 * M_PI / 180);
	camera->render = NULL;
	camera->z_buffer = NULL;
	return (camera);
}
