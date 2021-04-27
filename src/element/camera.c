#include "minirt.h"
#include "math.h"

t_camera	*parse_camera(t_list *data, t_rt_file file, char *type)
{
	t_vector3	pos;
	t_vector3	rot;
	int			fov;

	if (!args_size(type, data->size, 3))
		return (NULL);
	if (!vec_deserialize((char *)lst_get(data, 0), &pos)
		|| !dir_deserialize((char *)lst_get(data, 1), &rot)
		|| !bounded_int_deserialize((char *)lst_get(data, 2), &fov, 0, 180))
		return (NULL);
	return (new_camera(vec3_addv(pos, file.origin), rot, fov));
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
	c2w.e[0][0] = right.x;
	c2w.e[0][1] = right.y;
	c2w.e[0][2] = right.z;
	c2w.e[1][0] = up.x;
	c2w.e[1][1] = up.y;
	c2w.e[1][2] = up.z;
	c2w.e[2][0] = forward.x;
	c2w.e[2][1] = forward.y;
	c2w.e[2][2] = forward.z;
	c2w.e[3][0] = from.x;
	c2w.e[3][1] = from.y;
	c2w.e[3][2] = from.z;
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

	camera = malloc(sizeof(t_camera));
	if (!camera)
		return (NULL);
	camera->shadows = 0;
	camera->position = position;
	camera->direction = direction;
	reload_camera(camera);
	camera->hlen = tan(fov * 0.5 * M_PI / 180);
	camera->render = NULL;
	camera->z_buffer = NULL;
	camera->show_triangles = FALSE;
	return (camera);
}
